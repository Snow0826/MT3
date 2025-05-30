#include <Novice.h>
#include "Matrix4x4.h"
#include "Camera.h"
#include "Draw.h"
#include "Collision.h"
#include <imgui.h>
#include <algorithm>

constexpr char kWindowTitle[] = "LE2A_03_クラタ_ユウキ_MT3_02_00";
constexpr int32_t kWindowWidth = 1280;	// ウィンドウの幅
constexpr int32_t kWindowHeight = 720;	// ウィンドウの高さ

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// カメラの初期化
	Camera camera;
	camera.Initialize();

	// 制御点の初期化
	Sphere controlPoints[3] = {
		{ { -0.8f, 0.58f, 1.0f }, 0.01f },
		{ {  1.76f, 1.0f, -0.3f }, 0.01f },
		{ {  0.94f, -0.7f, 2.3f }, 0.01f }
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// カメラの更新
		camera.Update(keys, preKeys);

		// ImGuiの設定
		ImGui::Begin("Debug");

		if(ImGui::TreeNode("Control Points")) {
			for (int i = 0; i < 3; ++i) {
				ImGui::PushID(i);
				ImGui::DragFloat3("Position", &controlPoints[i].center.x, 0.01f, -10.0f, 10.0f);
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		ImGui::End();

		// レンダリングパイプライン
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = camera.GetViewMatrix() * projectionMatrix;
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		// ベジェ曲線を描画
		DrawBezier(controlPoints[0].center, controlPoints[1].center, controlPoints[2].center, viewProjectionMatrix, viewportMatrix, BLUE);

		// 制御点を描画
		for (const auto& point : controlPoints) {
			DrawSphere(point, viewProjectionMatrix, viewportMatrix, BLACK);
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
