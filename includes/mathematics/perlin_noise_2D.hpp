#ifndef PERLIN_NOISE_2D_HPP
#define PERLIN_NOISE_2D_HPP

#include "ivector2.hpp"

/**
 * @brief Generates 2D Perlin noise.
 *
 * Uses a bit-hash function to generate pseudo-random unit gradients per grid
 * node and cubic smoothstep interpolation.
 */
class PerlinNoise2D {
public:
	/**
	 * @brief Generates a Perlin noise value for the given coordinates.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @return Noise value approximately in the range [-1, 1].
	 */
	float operator()(float x, float y) const;

	/**
	 * @brief Returns a Perlin noise value for the given coordinates.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @return Noise value approximately in the range [-1, 1].
	 */
	float sample(float x, float y) const;

private:
	/**
	 * @brief Cubic smoothstep - smooths interpolation weight t.
	 * @param t Interpolation weight in [0, 1].
	 * @return Smoothed value: 3t^2 - 2t^3.
	 */
	float fade(const float t) const;

	/**
	 * @brief Linear interpolation between a0 and a1.
	 * @param a0 Start value.
	 * @param a1 End value.
	 * @param t Interpolation weight in [0, 1].
	 * @return Value between a0 and a1.
	 */
	float lerp(const float a0, const float a1, const float t) const;

	/**
	 * @brief Returns a pseudo-random unit gradient vector for a grid node.
	 *
	 * Uses a bit-hash of the grid coordinates to derive a random angle,
	 * then returns the corresponding unit vector (sin, cos).
	 *
	 * @param ix X coordinate of the grid node.
	 * @param iy Y coordinate of the grid node.
	 * @return Unit vector in a pseudo-random direction.
	 */
	IVector2<float> randomGradient(const int ix, const int iy) const;

	/**
	 * @brief Computes the dot product of the gradient at a grid node with
	 *        the distance vector from that node to point (x, y).
	 * @param ix X coordinate of the grid node.
	 * @param iy Y coordinate of the grid node.
	 * @param x X coordinate of the point.
	 * @param y Y coordinate of the point.
	 * @return Dot product value.
	 */
	float dotGridGradient(int ix, int iy, float x, float y) const;
};

#endif
