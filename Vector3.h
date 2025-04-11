#pragma once
#include <cmath>
#include <Novice.h>

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

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

};

inline constexpr Vector3 operator*(const float &s, const Vector3 &v) { return { s * v.x, s * v.y, s * v.z }; }
inline constexpr Vector3 operator/(const float &s, const Vector3 &v) { return { s / v.x, s / v.y, s / v.z }; }

static const int kColumnWidth = 60;

inline void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}