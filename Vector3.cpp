#include "Vector3.h"
#include <Novice.h>

static const int kColumnWidth = 60;

void Vector3::operator+=(const Vector3 &other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

void Vector3::operator-=(const Vector3 &other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
}

void Vector3::operator*=(const float &s) {
	x *= s;
	y *= s;
	z *= s;
}

void Vector3::operator/=(const float &s) {
	x /= s;
	y /= s;
	z /= s;
}

void Vector3::ScreenPrintf(int w, int h, const char *label) const {
	Novice::ScreenPrintf(w, h, "%.02f", x);
	Novice::ScreenPrintf(w + kColumnWidth, h, "%.02f", y);
	Novice::ScreenPrintf(w + kColumnWidth * 2, h, "%.02f", z);
	Novice::ScreenPrintf(w + kColumnWidth * 3, h, "%s", label);

}