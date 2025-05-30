#include "Math.h"

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return t * v1 + (1.0f - t) * v2;
}