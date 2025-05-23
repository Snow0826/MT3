#pragma once
#include <cassert>
#include "Vector3.h"

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
	Vector3 operator*(const Vector3 &other) const;
	Matrix4x4 operator-() const;
	Matrix4x4 transpose() const;
	Matrix4x4 inverse() const;
	void ScreenPrintf(int x, int y, const char *label) const;

	float m[4][4] = { 0.0f };

};

inline constexpr Vector3 operator*(const Vector3 &vector, const Matrix4x4 &matrix) {
	Vector3 result;
	result.x = matrix.m[0][0] * vector.x + matrix.m[1][0] * vector.y + matrix.m[2][0] * vector.z + matrix.m[3][0];
	result.y = matrix.m[0][1] * vector.x + matrix.m[1][1] * vector.y + matrix.m[2][1] * vector.z + matrix.m[3][1];
	result.z = matrix.m[0][2] * vector.x + matrix.m[1][2] * vector.y + matrix.m[2][2] * vector.z + matrix.m[3][2];
	float w = matrix.m[0][3] * vector.x + matrix.m[1][3] * vector.y + matrix.m[2][3] * vector.z + matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

inline constexpr Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
	return result;
}

inline constexpr Matrix4x4 MakeScaleMatrix(const Vector3 &scale) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	return result;
}

inline Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	return result;
}

inline Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = std::cos(radian);
	result.m[0][2] = -std::sin(radian);
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	return result;
}

inline Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	return result;
}

inline Matrix4x4 MakeRotateMatrix(const Vector3 &rotate) {
	return MakeRotateXMatrix(rotate.x) * MakeRotateYMatrix(rotate.y) * MakeRotateZMatrix(rotate.z);
}

inline constexpr Matrix4x4 MakeTranslateMatrix(const Vector3 &translate) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}

inline Matrix4x4 MakeSRTMatrix(const Vector3 &scale, const Vector3 &rotate, const Vector3 &translate) {
	return MakeScaleMatrix(scale) * MakeRotateMatrix(rotate) * MakeTranslateMatrix(translate);
}

inline Matrix4x4 MakeSTRMatrix(const Vector3 &scale, const Vector3 &translate, const Vector3 &rotate) {
	return MakeScaleMatrix(scale) * MakeTranslateMatrix(translate) * MakeRotateMatrix(rotate);
}

inline Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result;
	float f = 1.0f / std::tan(fovY / 2.0f);
	float d = farClip - nearClip;
	result.m[0][0] = f / aspectRatio;
	result.m[1][1] = f;
	result.m[2][2] = farClip / d;
	result.m[2][3] = 1.0f;
	result.m[3][2] = -nearClip * farClip / d;
	return result;
}

inline constexpr Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	return result;
}

inline constexpr Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;
	return result;
}