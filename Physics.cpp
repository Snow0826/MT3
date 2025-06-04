#include "Physics.h"

Vector3 Reflect(const Vector3 &input, const Vector3 &normal) {
	return input - 2.0f * (input.dot(normal)) * normal;
}