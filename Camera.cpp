#include "Camera.h"
#include <Novice.h>
#include <imgui.h>

void Camera::Initialize() {
	// カメラの初期化
	translation_ = { 0.0f, 0.19f, -6.49f };
	rotation_ = { 0.26f, 0.0f, 0.0f };
}

void Camera::Update(char *keys, char *preKeys) {
	// マウスの前フレームの位置を更新
	preMouseX_ = mouseX_;
	preMouseY_ = mouseY_;

	if (keys[DIK_LSHIFT] && !preKeys[DIK_LSHIFT] || Novice::IsTriggerMouse(2)) {
		Novice::GetMousePosition(&preMouseX_, &preMouseY_);	// マウスの前フレームの位置を取得
	}

	// カメラの移動と回転
	if (Novice::IsPressMouse(2)) {
		Novice::GetMousePosition(&mouseX_, &mouseY_);	// マウスの現在フレームの位置を取得
		if (keys[DIK_LSHIFT]) {
			translation_.x -= (mouseX_ - preMouseX_) * 0.01f;
			translation_.y += (mouseY_ - preMouseY_) * 0.01f;
		} else {
			rotation_.y -= (mouseX_ - preMouseX_) * 0.01f;
			rotation_.x += (mouseY_ - preMouseY_) * 0.01f;
		}
	}

	// カメラのズーム
	if (Novice::GetWheel() > 0) {
		translation_.z += 0.1f;
	} else if (Novice::GetWheel() < 0) {
		translation_.z -= 0.1f;
	}

	// ビュー行列の作成
	Matrix4x4 cameraMatrix = MakeSTRMatrix({ 1.0f, 1.0f, 1.0f }, translation_, rotation_);
	viewMatrix_ = cameraMatrix.inverse();
}