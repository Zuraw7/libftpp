#include "../test_utils.hpp"
#include "../../includes/mathematics/random_2D_coordinate_generator.hpp"

void random_2D_coordinate_generator_tests() {
	TEST_SECTION("Random2DCoordinateGenerator");

	// seed() returns the constructor seed
	{
		Random2DCoordinateGenerator gen(12345);
		EXPECT(gen.seed() == 12345);
	}

	// deterministic: same coordinates give the same value
	{
		Random2DCoordinateGenerator gen(42);
		EXPECT(gen(10, 20) == gen(10, 20));
		EXPECT(gen(-3, 7) == gen(-3, 7));
	}

	// two generators with the same seed agree
	{
		Random2DCoordinateGenerator a(99);
		Random2DCoordinateGenerator b(99);
		EXPECT(a(5, 6) == b(5, 6));
		EXPECT(a(-1, -1) == b(-1, -1));
	}

	// different seeds produce different values for the same coordinates
	{
		Random2DCoordinateGenerator a(1);
		Random2DCoordinateGenerator b(2);
		EXPECT(a(8, 8) != b(8, 8));
	}

	// different coordinates produce different values
	{
		Random2DCoordinateGenerator gen(7);
		EXPECT(gen(1, 2) != gen(3, 4));
	}

	// coordinate order matters: (x, y) differs from (y, x)
	{
		Random2DCoordinateGenerator gen(7);
		EXPECT(gen(1, 2) != gen(2, 1));
	}
}
