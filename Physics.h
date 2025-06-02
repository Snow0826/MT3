#pragma once
#include "Vector3.h"

/// @brief ばね
struct Spring {
	Vector3 anchor;	// アンカー。固定された端の位置
	float naturalLength;	// 自然長。スプリングが伸びていない状態での長さ
	float stiffness;	// バネ定数。スプリングの硬さを表す値
	float dampingCoefficient;	// 減衰係数。スプリングの動きに対する抵抗を表す値
};

/// @brief ボール
struct Ball {
	Vector3 position;	// ボールの位置
	Vector3 velocity;	// ボールの速度
	Vector3 acceleration;	// ボールの加速度
	float mass;	// ボールの質量
	float radius;	// ボールの半径
	uint32_t color;	// ボールの色
};