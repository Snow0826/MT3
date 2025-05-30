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

	// AABB1の初期化
	AABB aabb1{
		.min{-0.5f, -0.5f, -0.5f},	// 最小点
		.max{0.0f, 0.0f, 0.0f}	// 最大点
	};

	// AABB2の初期化
	AABB aabb2{
		.min{0.2f, 0.2f, 0.2f},	// 最小点
		.max{1.0f, 1.0f, 1.0f}	// 最大点
	};

	// 色の初期化
	uint32_t aabb1Color = WHITE;
	uint32_t aabb2Color = WHITE;

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

		if(ImGui::TreeNode("AABB1")) {
			ImGui::DragFloat3("Min", &aabb1.min.x, 0.01f);
			ImGui::DragFloat3("Max", &aabb1.max.x, 0.01f);
			// 最小点は最大点以下にする
			aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
			aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
			aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
			// 最大点は最小点以上にする
			aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
			aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
			aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);
			ImGui::TreePop();
		}

		if(ImGui::TreeNode("AABB2")) {
			ImGui::DragFloat3("Min", &aabb2.min.x, 0.01f);
			ImGui::DragFloat3("Max", &aabb2.max.x, 0.01f);
			// 最小点は最大点以下にする
			aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
			aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
			aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
			// 最大点は最小点以上にする
			aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
			aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
			aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);
			ImGui::TreePop();
		}

		ImGui::End();

		// AABB1とAABB2の衝突判定
		if (isCollision(aabb1, aabb2)) {
			aabb1Color = RED;
		} else {
			aabb1Color = WHITE;
		}

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

		// AABB1を描画
		DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, aabb1Color);

		// AABB2を描画
		DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, aabb2Color);

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
