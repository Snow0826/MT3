﻿#include "Collision.h"
#include "Draw.h"
#include <algorithm>

Vector3 Project(const Vector3 &v1, const Vector3 &v2) {
	Vector3 v2n = v2.normalized();
	float d = v1.dot(v2n);
	return v2n * d;
}

Vector3 ClosestPoint(const Vector3 &point, const Line &line) {
	float t = (point - line.origin).dot(line.diff) / line.diff.lengthSquare();
	return line.origin + t * line.diff;
}

Vector3 ClosestPoint(const Vector3 &point, const Ray &ray) {
	float t = (point - ray.origin).dot(ray.diff) / ray.diff.lengthSquare();
	t = std::max(t, 0.0f);
	return ray.origin + t * ray.diff;
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

bool isCollision(const Sphere &sphere, const Plane &plane) {
	float distance = std::abs(plane.normal.dot(sphere.center) - plane.distance);
	return distance <= sphere.radius;
}

bool isCollision(const Line &line, const Plane &plane) {
	float dot = plane.normal.dot(line.diff);
	if (dot == 0.0f) {
		return false; // 平行
	}
	return true;
}

bool isCollision(const Ray &ray, const Plane &plane) {
	float dot = plane.normal.dot(ray.diff);
	if (dot == 0.0f) {
		return false; // 平行
	}
	float t = (plane.distance - plane.normal.dot(ray.origin)) / dot;
	return t >= 0.0f;
}

bool isCollision(const Segment &segment, const Plane &plane) {
	float dot = plane.normal.dot(segment.diff);
	if (dot == 0.0f) {
		return false; // 平行
	}
	float t = (plane.distance - plane.normal.dot(segment.origin)) / dot;
	return t >= 0.0f && t <= 1.0f;
}

bool isCollision(const Triangle &triangle, const Line &line) {
	Plane plane;
	plane.normal = (triangle.vertices[1] - triangle.vertices[0]).cross(triangle.vertices[2] - triangle.vertices[0]);
	plane.normal = plane.normal.normalized();
	plane.distance = plane.normal.dot(triangle.vertices[0]);
	if (isCollision(line, plane)) {
		Vector3 closestPoint = ClosestPoint(line.origin, line);
		Vector3 edge[3];
		Vector3 cross[3];
		for (uint32_t i = 0; i < 3; ++i) {
			edge[i] = triangle.vertices[(i + 1) % 3] - triangle.vertices[i];
			cross[i] = edge[i].cross(closestPoint - triangle.vertices[i]);
			if (cross[i].dot(plane.normal) < 0.0f) {
				return false;
			}
		}
		return true;
	}
	return false;
}
bool isCollision(const Triangle &triangle, const Ray &ray) {
	Plane plane;
	plane.normal = (triangle.vertices[1] - triangle.vertices[0]).cross(triangle.vertices[2] - triangle.vertices[0]);
	plane.normal = plane.normal.normalized();
	plane.distance = plane.normal.dot(triangle.vertices[0]);
	if (isCollision(ray, plane)) {
		Vector3 closestPoint = ClosestPoint(ray.origin, ray);
		Vector3 edge[3];
		Vector3 cross[3];
		for (uint32_t i = 0; i < 3; ++i) {
			edge[i] = triangle.vertices[(i + 1) % 3] - triangle.vertices[i];
			cross[i] = edge[i].cross(closestPoint - triangle.vertices[i]);
			if (cross[i].dot(plane.normal) < 0.0f) {
				return false;
			}
		}
		return true;
	}
	return false;
}
bool isCollision(const Triangle &triangle, const Segment &segment) {
	Plane plane;
	plane.normal = (triangle.vertices[1] - triangle.vertices[0]).cross(triangle.vertices[2] - triangle.vertices[0]);
	plane.normal = plane.normal.normalized();
	plane.distance = plane.normal.dot(triangle.vertices[0]);
	if (isCollision(segment, plane)) {
		Vector3 closestPoint = ClosestPoint(segment.origin, segment);
		Vector3 edge[3];
		Vector3 cross[3];
		for (uint32_t i = 0; i < 3; ++i) {
			edge[i] = triangle.vertices[(i + 1) % 3] - triangle.vertices[i];
			cross[i] = edge[i].cross(closestPoint - triangle.vertices[i]);
			if (cross[i].dot(plane.normal) < 0.0f) {
				return false;
			}
		}
		return true;
	}
	return false;
}