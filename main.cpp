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

	// AABBの初期化
	AABB aabb{
		.min{-0.5f, -0.5f, -0.5f},	// 最小点
		.max{0.0f, 0.0f, 0.0f}	// 最大点
	};

	// 球の初期化
	Sphere sphere{
		.center{1.0f, 1.0f, 1.0f},	// 中心点
		.radius{1.0f}			// 半径
	};

	// 色の初期化
	uint32_t aabbColor = WHITE;
	uint32_t sphereColor = WHITE;

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

		if(ImGui::TreeNode("AABB")) {
			ImGui::DragFloat3("Min", &aabb.min.x, 0.01f);
			ImGui::DragFloat3("Max", &aabb.max.x, 0.01f);
			// 最小点は最大点以下にする
			aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
			aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
			aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
			// 最大点は最小点以上にする
			aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
			aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
			aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
			ImGui::TreePop();
		}

		if(ImGui::TreeNode("Sphere")) {
			ImGui::DragFloat3("Center", &sphere.center.x, 0.01f);
			ImGui::DragFloat("Radius", &sphere.radius, 0.01f);
			ImGui::TreePop();
		}

		ImGui::End();

		// AABBと球の衝突判定
		if (isCollision(aabb, sphere)) {
			aabbColor = RED;
		} else {
			aabbColor = WHITE;
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

		// AABBを描画
		DrawAABB(aabb, viewProjectionMatrix, viewportMatrix, aabbColor);

		// 球を描画
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, sphereColor);

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
