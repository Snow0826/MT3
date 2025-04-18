﻿#define _USE_MATH_DEFINES
#include "Draw.h"
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