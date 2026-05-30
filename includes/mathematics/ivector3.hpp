#ifndef IVECTOR3_HPP
#define IVECTOR3_HPP

#include <cmath>
#include <stdexcept>

/**
 * @brief A 3D vector with a templated component type.
 *
 * Supports arithmetic and comparison operators, as well as common
 * vector operations: length, normalization, dot product, and cross product.
 *
 * @tparam TType Numeric type of the vector components (e.g. int, float, double).
 */
template<typename TType>
struct IVector3 {
	TType x, y, z;

	/**
	 * @brief Constructs a vector with the given components.
	 * @param x First component.
	 * @param y Second component.
	 * @param z Third component.
	 */
	IVector3(const TType &x, const TType &y, const TType &z) : x(x), y(y), z(z) {}

	/**
	 * @brief Returns the component-wise sum of two vectors.
	 * @param other The vector to add.
	 * @return New vector equal to (x + other.x, y + other.y, z + other.z).
	 */
	IVector3<TType> operator+(const IVector3<TType>& other) {
		return IVector3<TType>(this->x + other.x, this->y + other.y, this->z + other.z);
	}

	/**
	 * @brief Returns the component-wise difference of two vectors.
	 * @param other The vector to subtract.
	 * @return New vector equal to (x - other.x, y - other.y, z - other.z).
	 */
	IVector3<TType> operator-(const IVector3<TType>& other) {
		return IVector3<TType>(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	/**
	 * @brief Returns the component-wise product of two vectors.
	 * @param other The vector to multiply by.
	 * @return New vector equal to (x * other.x, y * other.y, z * other.z).
	 */
	IVector3<TType> operator*(const IVector3<TType>& other) {
		return IVector3<TType>(this->x * other.x, this->y * other.y, this->z * other.z);
	}

	/**
	 * @brief Returns the component-wise quotient of two vectors.
	 * @param other The vector to divide by.
	 * @return New vector equal to (x / other.x, y / other.y, z / other.z).
	 * @throws std::invalid_argument if any component of other is zero.
	 */
	IVector3<TType> operator/(const IVector3<TType>& other) {
		if (other.x == 0 || other.y == 0 || other.z == 0)
			throw std::invalid_argument("IVector3: Division by zero");
		return IVector3<TType>(this->x / other.x, this->y / other.y, this->z / other.z);
	}

	/**
	 * @brief Returns true if all three components are equal.
	 * @param other The vector to compare against.
	 */
	bool operator==(const IVector3<TType>& other) {
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	/**
	 * @brief Returns true if any component differs.
	 * @param other The vector to compare against.
	 */
	bool operator!=(const IVector3<TType>& other) {
		return this->x != other.x || this->y != other.y || this->z != other.z;
	}

	/**
	 * @brief Returns the Euclidean length (L2 norm) of the vector.
	 * @return sqrt(x² + y² + z²).
	 */
	double length() {
		return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
	}

	/**
	 * @brief Returns the unit vector in the same direction.
	 * @return New IVector3<double> with length 1.
	 */
	IVector3<double> normalize() {
		double len = length();
		return IVector3<double>(x / len, y / len, z / len);
	}

	/**
	 * @brief Returns the dot product of two vectors.
	 * @param other The second vector.
	 * @return x * other.x + y * other.y + z * other.z.
	 */
	double dot(const IVector3 &other) {
		return x * other.x + y * other.y + z * other.z;
	}

	/**
	 * @brief Returns the cross product of two vectors.
	 * @param other The second vector.
	 * @return New vector perpendicular to both operands.
	 */
	IVector3<TType> cross(const IVector3 &other) {
		return IVector3<TType>(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}
};

#endif
