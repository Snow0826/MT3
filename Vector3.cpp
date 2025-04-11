#include "Vector3.h"

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
