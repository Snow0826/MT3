#pragma once
#include <Novice.h>

struct Matrix4x4 final {

	Matrix4x4 operator+(const Matrix4x4 &other) const;
	Matrix4x4 operator-(const Matrix4x4 &other) const;
	Matrix4x4 operator*(const Matrix4x4 &other) const;
	Matrix4x4 operator*(float other) const;
	Matrix4x4 operator/(float other) const;
	void operator+=(const Matrix4x4 &other);
	void operator-=(const Matrix4x4 &other);
	void operator*=(const Matrix4x4 &other);
	void operator*=(float other);
	void operator/=(float other);
	Matrix4x4 operator-() const;
	Matrix4x4 transpose() const;
	Matrix4x4 inverse() const;
	void ScreenPrintf(int x, int y, const char *label) const;

	float m[4][4] = { 0.0f };

};

inline constexpr Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
	return result;
}