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

struct Sphere;
struct Plane;

/// @brief 射影関数
/// @param v1 ベクトル1
/// @param v2 ベクトル2
/// @return 正射影ベクトル
Vector3 Project(const Vector3 &v1, const Vector3 &v2);

/// @brief 最近接点を求める関数
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