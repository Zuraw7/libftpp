#include "../test_utils.hpp"
#include "../../includes/mathematics/ivector3.hpp"

void ivector3_tests() {
	TEST_SECTION("IVector3");

	// component-wise arithmetic
	{
		IVector3<int> a(1, 2, 3);
		IVector3<int> b(4, 5, 6);
		EXPECT((a + b) == IVector3<int>(5, 7, 9));
		EXPECT((b - a) == IVector3<int>(3, 3, 3));
		EXPECT((a * b) == IVector3<int>(4, 10, 18));
	}

	// division, and division by zero throws (any component)
	{
		IVector3<int> a(8, 6, 4);
		IVector3<int> b(2, 3, 2);
		EXPECT((a / b) == IVector3<int>(4, 2, 2));

		IVector3<int> zero(1, 0, 1);
		EXPECT_THROW(a / zero);
	}

	// equality and inequality
	{
		IVector3<int> a(1, 2, 3);
		IVector3<int> same(1, 2, 3);
		IVector3<int> diff(1, 2, 9);
		EXPECT(a == same);
		EXPECT(!(a == diff));
		EXPECT(a != diff);
		EXPECT(!(a != same));
	}

	// length: 2² + 3² + 6² = 49 -> 7
	{
		IVector3<int> v(2, 3, 6);
		EXPECT_NEAR(v.length(), 7.0, 1e-9);
	}

	// normalize yields a unit vector in the same direction
	{
		IVector3<int> v(2, 3, 6);
		IVector3<double> n = v.normalize();
		EXPECT_NEAR(n.x, 2.0 / 7.0, 1e-9);
		EXPECT_NEAR(n.y, 3.0 / 7.0, 1e-9);
		EXPECT_NEAR(n.z, 6.0 / 7.0, 1e-9);
		EXPECT_NEAR(n.length(), 1.0, 1e-9);
	}

	// dot product
	{
		IVector3<int> a(1, 2, 3);
		IVector3<int> b(4, 5, 6);
		EXPECT_NEAR(a.dot(b), 32.0, 1e-9);   // 4 + 10 + 18
	}

	// cross product: x_hat x y_hat = z_hat
	{
		IVector3<int> x(1, 0, 0);
		IVector3<int> y(0, 1, 0);
		EXPECT(x.cross(y) == IVector3<int>(0, 0, 1));
	}
}
