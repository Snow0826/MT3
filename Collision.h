#pragma once
#include "Vector3.h"

/// @brief 線分
struct Segment {
	Vector3 origin;	// 始点
	Vector3 diff;	// 終点への差分ベクトル
};

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