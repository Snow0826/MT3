﻿#define _USE_MATH_DEFINES
#include "Draw.h"
#include "Math.h"
#include "Collision.h"
#include <Novice.h>

constexpr float pi = static_cast<float>(M_PI);	// 円周率

void DrawGrid(const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix) {
	constexpr float kGridHalfWidth = 2.0f;	// Gridの半分の幅
	constexpr uint32_t kSubdivision = 10;	// Gridの分割数
	constexpr float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubdivision);	// Gridの1マスの幅

	// 手前から奧に向かって描画
	for (float xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		
		// Gridの座標を計算
		Vector3 startPosition = { -kGridHalfWidth + (kGridEvery * xIndex), 0.0f, -kGridHalfWidth };
		Vector3 endPosition = { -kGridHalfWidth + (kGridEvery * xIndex), 0.0f, kGridHalfWidth};
		Vector3 screenStartPosition = startPosition * viewProjectionMatrix * viewportMatrix;
		Vector3 screenEndPosition = endPosition * viewProjectionMatrix * viewportMatrix;
		
		// Gridを描画
		if (xIndex == kSubdivision / 2) {
			Novice::DrawLine(
				static_cast<int32_t>(screenStartPosition.x),
				static_cast<int32_t>(screenStartPosition.y),
				static_cast<int32_t>(screenEndPosition.x),
				static_cast<int32_t>(screenEndPosition.y),
				BLACK);
		} else {
			Novice::DrawLine(
				static_cast<int32_t>(screenStartPosition.x),
				static_cast<int32_t>(screenStartPosition.y),
				static_cast<int32_t>(screenEndPosition.x),
				static_cast<int32_t>(screenEndPosition.y),
				0xAAAAAAFF);
		}
		
	}
	
	// 左から右に向かって描画
	for (float zIndex = 0.0f; zIndex <= kSubdivision; ++zIndex) {
		
		// Gridの座標を計算
		Vector3 startPosition = { -kGridHalfWidth, 0.0f, -kGridHalfWidth + (kGridEvery * zIndex) };
		Vector3 endPosition = { kGridHalfWidth, 0.0f, -kGridHalfWidth + (kGridEvery * zIndex) };
		Vector3 screenStartPosition = startPosition * viewProjectionMatrix * viewportMatrix;
		Vector3 screenEndPosition = endPosition * viewProjectionMatrix * viewportMatrix;
		
		// Gridを描画
		if (zIndex == kSubdivision / 2) {
			Novice::DrawLine(
				static_cast<int32_t>(screenStartPosition.x),
				static_cast<int32_t>(screenStartPosition.y),
				static_cast<int32_t>(screenEndPosition.x),
				static_cast<int32_t>(screenEndPosition.y),
				BLACK);
		} else {
			Novice::DrawLine(
				static_cast<int32_t>(screenStartPosition.x),
				static_cast<int32_t>(screenStartPosition.y),
				static_cast<int32_t>(screenEndPosition.x),
				static_cast<int32_t>(screenEndPosition.y),
				0xAAAAAAFF);
		}
	}
}

void DrawSphere(const Sphere &sphere, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	constexpr uint32_t kSubdivision = 16;	// 分割数
	constexpr float kLonEvery = pi * 2.0f / static_cast<float>(kSubdivision);	// 経度分割1つ分の角度
	constexpr float kLatEvery = pi / static_cast<float>(kSubdivision);	// 緯度分割1つ分の角度

	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;	// 現在の緯度

		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;	// 現在の経度
			float nextLon = lon + kLonEvery;
			float nextLat = lat + kLatEvery;

			// a,b,cのworld座標系を求める
			Vector3 a, b, c;
			a = { std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon) };
			b = { std::cos(nextLat) * std::cos(lon), std::sin(nextLat), std::cos(nextLat) * std::sin(lon) };
			c = { std::cos(lat) * std::cos(nextLon), std::sin(lat), std::cos(lat) * std::sin(nextLon) };
			a = sphere.center + sphere.radius * a;
			b = sphere.center + sphere.radius * b;
			c = sphere.center + sphere.radius * c;
			
			// a,b,cのscreen座標系を求める
			Vector3 screenA, screenB, screenC;
			screenA = a * viewProjectionMatrix * viewportMatrix;
			screenB = b * viewProjectionMatrix * viewportMatrix;
			screenC = c * viewProjectionMatrix * viewportMatrix;
			
			// abで線を引く
			Novice::DrawLine(
				static_cast<int32_t>(screenA.x),
				static_cast<int32_t>(screenA.y),
				static_cast<int32_t>(screenB.x),
				static_cast<int32_t>(screenB.y),
				color
			);
			
			// acで線を引く
			Novice::DrawLine(
				static_cast<int32_t>(screenA.x),
				static_cast<int32_t>(screenA.y),
				static_cast<int32_t>(screenC.x),
				static_cast<int32_t>(screenC.y),
				color
			);
		}
	}
}

Vector3 Perpendicular(const Vector3 &vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

void DrawPlane(const Plane &plane, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	Vector3 center = plane.normal * plane.distance;	// 平面の中心
	Vector3 perpendiculars[4];
	perpendiculars[0] = Perpendicular(plane.normal).normalized();	// 平面の法線に垂直なベクトル
	perpendiculars[1] = -perpendiculars[0];	// perpendiculars[0]の逆ベクトル
	perpendiculars[2] = perpendiculars[0].cross(plane.normal);	// 法線とperpendiculars[0]の外積
	perpendiculars[3] = -perpendiculars[2];	// perpendiculars[2]の逆ベクトル
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = perpendiculars[index] * 2.0f;
		Vector3 point = center + extend;
		points[index] = point * viewProjectionMatrix * viewportMatrix;
	}
	Novice::DrawLine(
		static_cast<int32_t>(points[0].x),
		static_cast<int32_t>(points[0].y),
		static_cast<int32_t>(points[2].x),
		static_cast<int32_t>(points[2].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(points[0].x),
		static_cast<int32_t>(points[0].y),
		static_cast<int32_t>(points[3].x),
		static_cast<int32_t>(points[3].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(points[1].x),
		static_cast<int32_t>(points[1].y),
		static_cast<int32_t>(points[2].x),
		static_cast<int32_t>(points[2].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(points[1].x),
		static_cast<int32_t>(points[1].y),
		static_cast<int32_t>(points[3].x),
		static_cast<int32_t>(points[3].y),
		color
	);
}

void DrawTriangle(const Triangle &triangle, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	Vector3 screenVertices[3];
	for (int32_t index = 0; index < 3; ++index) {
		screenVertices[index] = triangle.vertices[index] * viewProjectionMatrix * viewportMatrix;
	}
	Novice::DrawTriangle(
		static_cast<int32_t>(screenVertices[0].x),
		static_cast<int32_t>(screenVertices[0].y),
		static_cast<int32_t>(screenVertices[1].x),
		static_cast<int32_t>(screenVertices[1].y),
		static_cast<int32_t>(screenVertices[2].x),
		static_cast<int32_t>(screenVertices[2].y),
		color,
		kFillModeWireFrame
	);
}

void DrawAABB(const AABB &aabb, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	Vector3 vertices[8] = {
		{ aabb.min.x, aabb.min.y, aabb.min.z },
		{ aabb.max.x, aabb.min.y, aabb.min.z },
		{ aabb.max.x, aabb.max.y, aabb.min.z },
		{ aabb.min.x, aabb.max.y, aabb.min.z },
		{ aabb.min.x, aabb.min.y, aabb.max.z },
		{ aabb.max.x, aabb.min.y, aabb.max.z },
		{ aabb.max.x, aabb.max.y, aabb.max.z },
		{ aabb.min.x, aabb.max.y, aabb.max.z }
	};

	Vector3 screenVertices[8];
	for (int32_t index = 0; index < 8; ++index) {
		screenVertices[index] = vertices[index] * viewProjectionMatrix * viewportMatrix;
	}

	// 12本の線を引く
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[0].x),
		static_cast<int32_t>(screenVertices[0].y),
		static_cast<int32_t>(screenVertices[1].x),
		static_cast<int32_t>(screenVertices[1].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[1].x),
		static_cast<int32_t>(screenVertices[1].y),
		static_cast<int32_t>(screenVertices[2].x),
		static_cast<int32_t>(screenVertices[2].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[2].x),
		static_cast<int32_t>(screenVertices[2].y),
		static_cast<int32_t>(screenVertices[3].x),
		static_cast<int32_t>(screenVertices[3].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[3].x),
		static_cast<int32_t>(screenVertices[3].y),
		static_cast<int32_t>(screenVertices[0].x),
		static_cast<int32_t>(screenVertices[0].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[4].x),
		static_cast<int32_t>(screenVertices[4].y),
		static_cast<int32_t>(screenVertices[5].x),
		static_cast<int32_t>(screenVertices[5].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[5].x),
		static_cast<int32_t>(screenVertices[5].y),
		static_cast<int32_t>(screenVertices[6].x),
		static_cast<int32_t>(screenVertices[6].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[6].x),
		static_cast<int32_t>(screenVertices[6].y),
		static_cast<int32_t>(screenVertices[7].x),
		static_cast<int32_t>(screenVertices[7].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[7].x),
		static_cast<int32_t>(screenVertices[7].y),
		static_cast<int32_t>(screenVertices[4].x),
		static_cast<int32_t>(screenVertices[4].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[0].x),
		static_cast<int32_t>(screenVertices[0].y),
		static_cast<int32_t>(screenVertices[4].x),
		static_cast<int32_t>(screenVertices[4].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[1].x),
		static_cast<int32_t>(screenVertices[1].y),
		static_cast<int32_t>(screenVertices[5].x),
		static_cast<int32_t>(screenVertices[5].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[2].x),
		static_cast<int32_t>(screenVertices[2].y),
		static_cast<int32_t>(screenVertices[6].x),
		static_cast<int32_t>(screenVertices[6].y),
		color
	);
	Novice::DrawLine(
		static_cast<int32_t>(screenVertices[3].x),
		static_cast<int32_t>(screenVertices[3].y),
		static_cast<int32_t>(screenVertices[7].x),
		static_cast<int32_t>(screenVertices[7].y),
		color
	);
}

void DrawBezier(const Vector3 &controlPoint0, const Vector3 &controlPoint1, const Vector3 &controlPoint2, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	constexpr uint32_t split = 32;
	for (uint32_t i = 0; i < split; i++) {
		float t0 = static_cast<float>(i) / static_cast<float>(split);
		float t1 = static_cast<float>(i + 1) / static_cast<float>(split);

		Vector3 p0 = Lerp(controlPoint0, controlPoint1, t0);
		Vector3 p1 = Lerp(controlPoint1, controlPoint2, t0);
		Vector3 bezier0 = Lerp(p0, p1, t0);

		p0 = Lerp(controlPoint0, controlPoint1, t1);
		p1 = Lerp(controlPoint1, controlPoint2, t1);
		Vector3 bezier1 = Lerp(p0, p1, t1);

		Vector3 start = bezier0 * viewProjectionMatrix * viewportMatrix;
		Vector3 end = bezier1 * viewProjectionMatrix * viewportMatrix;

		Novice::DrawLine(
			static_cast<int32_t>(start.x),
			static_cast<int32_t>(start.y),
			static_cast<int32_t>(end.x),
			static_cast<int32_t>(end.y),
			color);
	}
}