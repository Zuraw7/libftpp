#include "../test_utils.hpp"
#include "../../includes/mathematics/perlin_noise_2D.hpp"

void perlin_noise_2D_tests() {
	TEST_SECTION("PerlinNoise2D");

	PerlinNoise2D noise;

	// deterministic: same coordinates always give the same value
	{
		EXPECT(noise.sample(2.5f, 3.7f) == noise.sample(2.5f, 3.7f));
		EXPECT(noise(1.1f, -4.2f) == noise(1.1f, -4.2f));
	}

	// operator() and sample() agree
	{
		EXPECT(noise(2.5f, 3.7f) == noise.sample(2.5f, 3.7f));
		EXPECT(noise(-0.3f, 8.9f) == noise.sample(-0.3f, 8.9f));
	}

	// Perlin noise is ~0 at integer lattice points
	{
		EXPECT_NEAR(noise.sample(0.0f, 0.0f), 0.0f, 1e-5);
		EXPECT_NEAR(noise.sample(1.0f, 1.0f), 0.0f, 1e-5);
		EXPECT_NEAR(noise.sample(5.0f, 3.0f), 0.0f, 1e-5);
		EXPECT_NEAR(noise.sample(-2.0f, 4.0f), 0.0f, 1e-5);
	}

	// values stay within a sane range (approx [-1, 1], allow slight overshoot)
	{
		bool inRange = true;
		for (float x = 0.0f; x < 10.0f; x += 0.37f)
			for (float y = 0.0f; y < 10.0f; y += 0.41f) {
				float v = noise.sample(x, y);
				if (v < -1.1f || v > 1.1f)
					inRange = false;
			}
		EXPECT(inRange);
	}

	// not constant: different points produce different values
	{
		EXPECT(noise.sample(0.5f, 0.5f) != noise.sample(7.3f, 2.1f));
	}
}
