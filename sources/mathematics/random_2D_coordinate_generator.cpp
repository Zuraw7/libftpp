#include "../../includes/mathematics/random_2D_coordinate_generator.hpp"
#include <random>

Random2DCoordinateGenerator::Random2DCoordinateGenerator(long long seed) : m_seed(seed) { }

long long Random2DCoordinateGenerator::seed() const {
	return m_seed;
}

long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y) {
	std::mt19937_64 engine(m_seed ^ (x * std::mt19937_64::xor_mask) ^ (y * std::mt19937_64::initialization_multiplier));
	return engine();
}
