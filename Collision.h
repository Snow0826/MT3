#pragma once
#include "Vector3.h"

/// @brief 直線
struct Line {
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};

/// @brief 半直線
struct Ray {
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};;

/// @brief 線分
struct Segment {
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};

/// @brief カプセル
struct Capsule {
	Segment segment;	// 線分部分
	float radius;	// 半径
};

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

/// @brief 軸並行境界箱
struct AABB {
	Vector3 min;	// 最小点
	Vector3 max;	// 最大点
};

/// @brief 射影関数
/// @param v1 ベクトル1
/// @param v2 ベクトル2
/// @return 正射影ベクトル
Vector3 Project(const Vector3 &v1, const Vector3 &v2);

/// @brief 点と直線の最近接点を求める関数
/// @param point 点
/// @param line 直線
/// @return 最近接点
Vector3 ClosestPoint(const Vector3 &point, const Line &line);

/// @brief 点と半直線の最近接点を求める関数
/// @param point 点
/// @param ray 半直線
/// @return 最近接点
Vector3 ClosestPoint(const Vector3 &point, const Ray &ray);

/// @brief 点と線分の最近接点を求める関数
/// @param point 点
/// @param segment 線分
/// @return 最近接点
Vector3 ClosestPoint(const Vector3 &point, const Segment &segment);

/// @brief 球と球の衝突判定
/// @param sphere1 球1
/// @param sphere2 球2
/// @return 判定結果
bool isCollision(const Sphere &sphere1, const Sphere &sphere2);

/// @brief 球と平面の衝突判定
/// @param sphere 球
/// @param plane 平面
/// @return 判定結果
bool isCollision(const Sphere &sphere, const Plane &plane);

/// @brief 直線と平面の衝突判定
/// @param line 直線
/// @param plane 平面
/// @return 判定結果
bool isCollision(const Line &line, const Plane &plane);

/// @brief 半直線と平面の衝突判定
/// @param ray 半直線
/// @param plane 平面
/// @return 判定結果
bool isCollision(const Ray &ray, const Plane &plane);

/// @brief 線分と平面の衝突判定
/// @param segment 線分
/// @param plane 平面
/// @return 判定結果
bool isCollision(const Segment &segment, const Plane &plane);

/// @brief 三角形と直線の衝突判定
/// @param triangle 三角形
/// @param line 直線
/// @return 判定結果
bool isCollision(const Triangle &triangle, const Line &line);

/// @brief 三角形と半直線の衝突判定
/// @param triangle 三角形
/// @param ray 半直線
/// @return 判定結果
bool isCollision(const Triangle &triangle, const Ray &ray);

/// @brief 三角形と線分の衝突判定
/// @param triangle 三角形
/// @param segment 線分
/// @return 判定結果
bool isCollision(const Triangle &triangle, const Segment &segment);

/// @brief AABBとAABBの衝突判定
/// @param aabb1 AABB1
/// @param aabb2 AABB2
/// @return 判定結果
bool isCollision(const AABB &aabb1, const AABB &aabb2);

/// @brief AABBと球の衝突判定
/// @param aabb AABB
/// @param sphere 球
/// @return 判定結果
bool isCollision(const AABB &aabb, const Sphere &sphere);

/// @brief AABBと直線の衝突判定
/// @param aabb AABB
/// @param line 直線
/// @return 判定結果
bool isCollision(const AABB &aabb, const Line &line);

/// @brief AABBと半直線の衝突判定
/// @param aabb AABB
/// @param ray 半直線
/// @return 判定結果
bool isCollision(const AABB &aabb, const Ray &ray);

/// @brief AABBと線分の衝突判定
/// @param aabb AABB
/// @param segment 線分
/// @return 判定結果
bool isCollision(const AABB &aabb, const Segment &segment);

/// @brief カプセルと平面の衝突判定
/// @param capsule カプセル
/// @param plane 平面
/// @return 判定結果
bool isCollision(const Capsule &capsule, const Plane &plane);