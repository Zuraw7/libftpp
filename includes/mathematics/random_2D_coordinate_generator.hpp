#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
#define RANDOM_2D_COORDINATE_GENERATOR_HPP

/**
 * @brief Generates pseudo-random numbers based on 2D coordinates.
 *
 * The generator is deterministic - the same coordinates and seed
 * always produce the same result.
 */
class Random2DCoordinateGenerator {
public:
	/**
	 * @brief Constructs the generator with the given seed.
	 * @param seed Initial seed value controlling the output sequence.
	 */
	Random2DCoordinateGenerator(long long seed);

	/**
	 * @brief Returns the seed of the generator.
	 * @return The seed value set at construction.
	 */
	long long seed() const;

	/**
	 * @brief Generates a pseudo-random number from 2D coordinates.
	 *
	 * The same (x, y) pair always produces the same result for a given seed.
	 *
	 * @param x First coordinate.
	 * @param y Second coordinate.
	 * @return Pseudo-random 64-bit integer derived from x, y and the seed.
	 */
	long long operator()(const long long& x, const long long& y);

private:
	long long m_seed;
};

#endif
