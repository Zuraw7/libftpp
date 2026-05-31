#include "../../includes/mathematics/perlin_noise_2D.hpp"
#include <cmath>

float PerlinNoise2D::fade(const float t) const {
	return t * t * (3.0f - 2.0f * t);
}

float PerlinNoise2D::lerp(const float a0, const float a1, const float t) const {
	return a0 + (a1 - a0) * t;
}

IVector2<float> PerlinNoise2D::randomGradient(const int ix, const int iy) const {
    const unsigned int w = 8 * sizeof(unsigned int);
    const unsigned int s = w / 2; 
    unsigned int a = ix, b = iy;
    a *= 3284157443;
 
    b ^= a << s | a >> (w - s);
    b *= 1911520717;
 
    a ^= b << s | b >> (w - s);
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    
    // Return the vector from the angle
	return IVector2<float>(std::sin(random), std::cos(random));
}
 
// Computes the dot product of the distance and gradient vectors.
float PerlinNoise2D::dotGridGradient(int ix, int iy, float x, float y) const {
    IVector2<float> gradient = randomGradient(ix, iy);
 
    // Compute the distance vector
    float dx = x - static_cast<float>(ix);
    float dy = y - static_cast<float>(iy);
 
	return gradient.dot({dx, dy});
}
 
float PerlinNoise2D::operator()(float x, float y) const {
	// Determine grid cell corner coordinates
    int x0 = static_cast<int>(x); 
    int y0 = static_cast<int>(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;
 
    // Compute Interpolation weights
    float sx = x - static_cast<float>(x0);
    float sy = y - static_cast<float>(y0);
    
    // Compute and interpolate top two corners
    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = lerp(n0, n1, fade(sx));
 
    // Compute and interpolate bottom two corners
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = lerp(n0, n1, fade(sx));
 
    // Final step: interpolate between the two previously interpolated values, now in y
    float value = lerp(ix0, ix1, fade(sy));
    
    return value;
}

float PerlinNoise2D::sample(float x, float y) const {
	return (*this)(x, y);
}
