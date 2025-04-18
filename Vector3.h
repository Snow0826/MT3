#pragma once
#include <cmath>

struct Vector3 final {

	float length() const { return sqrtf(lengthSquare()); }
	constexpr float lengthSquare() const { return dot(*this); }
	constexpr float dot(const Vector3 &other) const { return this->x * other.x + this->y * other.y + this->z * other.z; }
	float distanceFrom(const Vector3 &other) const { return (other - *this).length(); }
	Vector3 normalized() const { return *this / length(); }
	Vector3 normalized(const Vector3 &other) const { return (other - *this) / distanceFrom(other); }
	constexpr bool isZero() const { return *this == Vector3{}; }
	constexpr bool operator==(const Vector3 &other) const { return this->x == other.x && this->y == other.y && this->z == other.z; }
	constexpr Vector3 operator+() const { return *this; }
	constexpr Vector3 operator-() const { return { -x, -y, -z }; }
	constexpr Vector3 operator+(const Vector3 &other) const { return { this->x + other.x, this->y + other.y, this->z + other.z }; }
	constexpr Vector3 operator-(const Vector3 &other) const { return { this->x - other.x, this->y - other.y, this->z - other.z }; }
	constexpr Vector3 operator*(const float &s) const { return { x * s, y * s, z * s }; }
	constexpr Vector3 operator/(const float &s) const { return { x / s, y / s, z / s }; }
	void operator+=(const Vector3 &other);
	void operator-=(const Vector3 &other);
	void operator*=(const float &s);
	void operator/=(const float &s);
	void ScreenPrintf(int w, int h, const char *label) const;

	constexpr Vector3 cross(const Vector3 &other) const {
		return {
			this->y * other.z - this->z * other.y,
			this->z * other.x - this->x * other.z,
			this->x * other.y - this->y * other.x
		};
	}

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

};

inline constexpr Vector3 operator*(const float &s, const Vector3 &v) { return { s * v.x, s * v.y, s * v.z }; }
inline constexpr Vector3 operator/(const float &s, const Vector3 &v) { return { s / v.x, s / v.y, s / v.z }; }
