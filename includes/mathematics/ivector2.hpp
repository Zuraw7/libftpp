#ifndef IVECTOR2_HPP
#define IVECTOR2_HPP

#include <cmath>
#include <stdexcept>

/**
 * @brief A 2D vector with a templated component type.
 *
 * Supports arithmetic and comparison operators, as well as common
 * vector operations: length, normalization, dot product, and cross product.
 *
 * @tparam TType Numeric type of the vector components (e.g. int, float, double).
 */
template<typename TType>
struct IVector2 {
	TType x;
	TType y;

	/**
	 * @brief Constructs a vector with the given components.
	 * @param x First component.
	 * @param y Second component.
	 */
	IVector2(const TType &x, const TType &y) : x(x), y(y) {}

	/**
	 * @brief Returns the component-wise sum of two vectors.
	 * @param other The vector to add.
	 * @return New vector equal to (x + other.x, y + other.y).
	 */
	IVector2<TType> operator+(const IVector2<TType>& other) {
		return IVector2<TType>(this->x + other.x, this->y + other.y);
	}

	/**
	 * @brief Returns the component-wise difference of two vectors.
	 * @param other The vector to subtract.
	 * @return New vector equal to (x - other.x, y - other.y).
	 */
	IVector2<TType> operator-(const IVector2<TType>& other) {
		return IVector2<TType>(this->x - other.x, this->y - other.y);
	}

	/**
	 * @brief Returns the component-wise product of two vectors.
	 * @param other The vector to multiply by.
	 * @return New vector equal to (x * other.x, y * other.y).
	 */
	IVector2<TType> operator*(const IVector2<TType>& other) {
		return IVector2<TType>(this->x * other.x, this->y * other.y);
	}

	/**
	 * @brief Returns the component-wise quotient of two vectors.
	 * @param other The vector to divide by.
	 * @return New vector equal to (x / other.x, y / other.y).
	 * @throws std::invalid_argument if either component of other is zero.
	 */
	IVector2<TType> operator/(const IVector2<TType>& other) {
		if (other.x == 0 || other.y == 0)
			throw std::invalid_argument("IVector2: Division by zero");
		return IVector2<TType>(this->x / other.x, this->y / other.y);
	}

	/**
	 * @brief Returns true if both components are equal.
	 * @param other The vector to compare against.
	 */
	bool operator==(const IVector2<TType>& other) {
		return this->x == other.x && this->y == other.y;
	}

	/**
	 * @brief Returns true if any component differs.
	 * @param other The vector to compare against.
	 */
	bool operator!=(const IVector2<TType>& other) {
		return this->x != other.x || this->y != other.y;
	}

	/**
	 * @brief Returns the Euclidean length (L2 norm) of the vector.
	 * @return sqrt(pow(x) + pow(y)).
	 */
	double length() {
		return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
	}

	/**
	 * @brief Returns the unit vector in the same direction.
	 * @return New IVector2<double> with length 1.
	 */
	IVector2<double> normalize() {
		double len = length();
		return IVector2<double>(x / len, y / len);
	}

	/**
	 * @brief Returns the dot product of two vectors.
	 * @param other The second vector.
	 * @return x * other.x + y * other.y.
	 */
	double dot(const IVector2 &other) {
		return this->x * other.x + this->y * other.y;
	}

	/**
	 * @brief Returns the 2D cross product (scalar) of two vectors.
	 * @param other The second vector.
	 * @return x * other.y - y * other.x.
	 */
	double cross(const IVector2 &other) {
		return this->x * other.y - this->y * other.x;
	}
};

#endif
