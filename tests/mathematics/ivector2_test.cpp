#include "../test_utils.hpp"
#include "../../includes/mathematics/ivector2.hpp"

void ivector2_tests() {
	TEST_SECTION("IVector2");

	// component-wise arithmetic
	{
		IVector2<int> a(2, 3);
		IVector2<int> b(4, 5);
		EXPECT((a + b) == IVector2<int>(6, 8));
		EXPECT((b - a) == IVector2<int>(2, 2));
		EXPECT((a * b) == IVector2<int>(8, 15));
	}

	// division, and division by zero throws
	{
		IVector2<int> a(8, 6);
		IVector2<int> b(2, 3);
		EXPECT((a / b) == IVector2<int>(4, 2));

		IVector2<int> zero(0, 1);
		EXPECT_THROW(a / zero);
	}

	// equality and inequality
	{
		IVector2<int> a(1, 2);
		IVector2<int> same(1, 2);
		IVector2<int> diff(1, 9);
		EXPECT(a == same);
		EXPECT(!(a == diff));
		EXPECT(a != diff);
		EXPECT(!(a != same));
	}

	// length: 3-4-5 right triangle
	{
		IVector2<int> v(3, 4);
		EXPECT_NEAR(v.length(), 5.0, 1e-9);
	}

	// normalize yields a unit vector in the same direction
	{
		IVector2<int> v(3, 4);
		IVector2<double> n = v.normalize();
		EXPECT_NEAR(n.x, 0.6, 1e-9);
		EXPECT_NEAR(n.y, 0.8, 1e-9);
		EXPECT_NEAR(n.length(), 1.0, 1e-9);
	}

	// dot product
	{
		IVector2<int> a(1, 2);
		IVector2<int> b(3, 4);
		EXPECT_NEAR(a.dot(b), 11.0, 1e-9);   // 1*3 + 2*4
	}

	// 2D cross product (scalar)
	{
		IVector2<int> a(1, 0);
		IVector2<int> b(0, 1);
		EXPECT_NEAR(a.cross(b), 1.0, 1e-9);  // 1*1 - 0*0
	}
}
