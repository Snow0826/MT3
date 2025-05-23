#pragma once
#include "Matrix4x4.h"

class Camera {
public:
	/// @brief 初期化
	void Initialize();

	/// @brief 更新
	/// @param keys 現在フレームのキー入力
	/// @param preKeys 前フレームのキー入力
	void Update(char *keys, char *preKeys);

	/// @brief ビュー行列を取得
	/// @return ビュー行列
	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }

private:
	int32_t preMouseX_ = 0;	// 前フレームのマウスのX座標
	int32_t preMouseY_ = 0;	// 前フレームのマウスのY座標
	int32_t mouseX_ = 0;	// 現在フレームのマウスのX座標
	int32_t mouseY_ = 0;	// 現在フレームのマウスのY座標

	Vector3 translation_;	// 位置
	Vector3 rotation_;	// 向き

	Matrix4x4 viewMatrix_;	// ビュー行列
};

