#pragma once
#include "Matrix4x4.h"

/// @brief 球
struct Sphere {
	Vector3 center;	// 中心点
	float radius;	// 半径
};

/// @brief 平面
struct Plane {
	Vector3 normal;	// 法線ベクトル
	float distance;	// 原点からの距離
};;

/// @brief 三角形
struct Triangle {
	Vector3 vertices[3];	// 頂点
};

struct AABB {
	Vector3 min;	// 最小点
	Vector3 max;	// 最大点
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

/// @brief 垂直なベクトルを求める関数
/// @param vector ベクトル
/// @return 垂直なベクトル
Vector3 Perpendicular(const Vector3 &vector);

/// @brief 平面の描画関数
/// @param plane 描画する平面
/// @param viewProjectionMatrix vp行列
/// @param viewportMatrix viewport変換行列
/// @param color 描画する色
void DrawPlane(const Plane &plane, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

/// @brief 三角形の描画関数
/// @param triangle 三角形
/// @param viewProjectionMatrix vp行列
/// @param viewportMatrix viewport変換行列
/// @param color 描画する色
void DrawTriangle(const Triangle &triangle, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

/// @brief AABBの描画関数
/// @param aabb AABB
/// @param viewProjectionMatrix vp行列
/// @param viewportMatrix viewport変換行列
/// @param color 描画する色
void DrawAABB(const AABB &aabb, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);