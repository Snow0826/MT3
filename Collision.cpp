#include "Collision.h"
#include "Draw.h"
#include <algorithm>

Vector3 Project(const Vector3 &v1, const Vector3 &v2) {
	Vector3 v2n = v2.normalized();
	float d = v1.dot(v2n);
	return v2n * d;
}
	
Vector3 ClosestPoint(const Vector3 &point, const Segment &segment) {
	float t = (point - segment.origin).dot(segment.diff) / segment.diff.lengthSquare();
	t = std::clamp(t, 0.0f, 1.0f);
	return segment.origin + t * segment.diff;
}

bool isCollision(const Sphere &sphere1, const Sphere &sphere2) {
	float distance = sphere1.center.distanceFrom(sphere2.center);
	return distance <= sphere1.radius + sphere2.radius;
}