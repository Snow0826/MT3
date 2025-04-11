#include "Matrix4x4.h"
#include <Novice.h>

constexpr float kMax_err = 1.0E-10f;
static const int kRowHeight = 20;
static const int kColumnWidth = 60;

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &other) const {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = this->m[i][j] + other.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 &other) const {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = this->m[i][j] - other.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += this->m[i][k] * other.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::operator*(float other) const {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = this->m[i][j] * other;
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::operator/(float other) const {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = this->m[i][j] / other;
		}
	}
	return result;
}

void Matrix4x4::operator+=(const Matrix4x4 &other) {
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			this->m[i][j] += other.m[i][j];
		}
	}
}

void Matrix4x4::operator-=(const Matrix4x4 &other) {
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			this->m[i][j] -= other.m[i][j];
		}
	}
}

void Matrix4x4::operator*=(const Matrix4x4 &other) {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += this->m[i][k] * other.m[k][j];
			}
		}
	}
	*this = result;
}

void Matrix4x4::operator*=(float other) {
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			m[i][j] *= other;
		}
	}
}

void Matrix4x4::operator/=(float other) {
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			m[i][j] /= other;
		}
	}
}

Vector3 Matrix4x4::operator*(const Vector3 &other) const {
	Vector3 result;
	result.x = m[0][0] * other.x + m[1][0] * other.y + m[2][0] * other.z + m[3][0];
	result.y = m[0][1] * other.x + m[1][1] * other.y + m[2][1] * other.z + m[3][1];
	result.z = m[0][2] * other.x + m[1][2] * other.y + m[2][2] * other.z + m[3][2];
	float w = m[0][3] * other.x + m[1][3] * other.y + m[2][3] * other.z + m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 Matrix4x4::operator-() const {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = -this->m[i][j];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::transpose() const {
	Matrix4x4 result;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = this->m[j][i];
		}
	}
	return result;
}

Matrix4x4 Matrix4x4::inverse() const {

	Matrix4x4 result;
	float A[4][8] = { 0 };
	size_t n = 4;
	float a;

	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			A[i][j] = m[i][j];
			A[i][j + n] = (i == j) ? 1.0f : 0.0f;
		}
	}

	for (size_t i = 0; i < n; i++) {

		float max = fabsf(A[i][i]);
		size_t max_i = i;

		for (size_t j = i + 1; j < n; j++) {
			if (fabsf(A[i][i]) > max) {
				max = fabsf(A[i][i]);
				max_i = i;
			}
		}

		if (fabsf(A[i][i]) <= kMax_err) {
			break;
		}

		if (i != max_i) {
			for (size_t j = 0; j < n * 2; j++) {
				float tmp = A[max_i][j];
				A[max_i][j] = A[i][j];
				A[i][j] = tmp;
			}
		}

		a = 1.0f / A[i][i];

		for (size_t j = 0; j < n * 2; j++) {
			A[i][j] *= a;
		}

		for (size_t j = 0; j < n; j++) {

			if (i == j) {
				continue;
			}

			a = -A[j][i];

			for (size_t k = 0; k < n * 2; k++) {
				A[j][k] += A[i][k] * a;
			}

		}

	}

	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			result.m[i][j] = A[i][j + n];
		}
	}

	return result;

}

void Matrix4x4::ScreenPrintf(int x, int y, const char* label) const {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (uint32_t row = 0; row < 4; row++) {
		for (uint32_t column = 0; column < 4; column++) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", m[row][column]);
		}
	}
}