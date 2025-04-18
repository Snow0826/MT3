#pragma once
#include "Matrix4x4.h"

/// @brief 球
struct Sphere {
	Vector3 center;	// 中心点
	float radius;	// 半径
};

/// @brief グリッドの描画関数
/// @param viewProjectionMatrix vp行列
/// @param viewportMatrix viewport変換行列
void DrawGrid(const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix);

/// @brief 球の描画関数
/// @param sphere 描画する球
/// @param viewProjectionMatrix vp行列
/// @param viewportMatrix viewport変換行列
/// @param color 描画する色
void DrawSphere(const Sphere &sphere, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);