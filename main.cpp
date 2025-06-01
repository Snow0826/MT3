#include <Novice.h>
#include "Camera.h"
#include "Draw.h"
#include "Collision.h"
#include <imgui.h>
#include <string>

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

	// ローカル座標系の肩、肘、手の球の初期化
	Sphere localSpheres[3];
	for (Sphere &sphere : localSpheres) {
		sphere = { { 0.0f, 0.0f, 0.0f }, 0.1f };
	}

	// 肩、肘、手の位置の初期化
	Vector3 translates[3] = {
		{ 0.2f, 1.0f, 0.0f },
		{ 0.4f, 0.0f, 0.0f },
		{ 0.3f, 0.0f, 0.0f }
	};

	// 肩、肘、手の向きの初期化
	Vector3 rotates[3] = {
		{ 0.0f, 0.0f, -6.8f },
		{ 0.0f, 0.0f, -1.4f },
		{ 0.0f, 0.0f,  0.0f }
	};

	// 肩、肘、手の大きさの初期化
	Vector3 scales[3] = {
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f }
	};

	// 肩、肘、手の色の初期化
	uint32_t colors[3] = { RED, GREEN, BLUE };

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

		if(ImGui::TreeNode("Hierarchy")) {

			std::string lables[3] = {
				"Shoulder",
				"Elbow",
				"Hand"
			};

			for (uint32_t i = 0; i < 3; ++i) {
				if (ImGui::TreeNode(lables[i].c_str())) {
					ImGui::DragFloat3("Position", &translates[i].x, 0.01f, -10.0f, 10.0f);
					ImGui::DragFloat3("Rotation", &rotates[i].x, 0.1f, -180.0f, 180.0f);
					ImGui::DragFloat3("Scale", &scales[i].x, 0.01f, 0.1f, 10.0f);
					ImGui::TreePop();
				}
			}
			
			ImGui::TreePop();
		}

		ImGui::End();

		// ローカル行列を作成
		Matrix4x4 localMatrix[3];
		for(uint32_t i = 0; i < 3; ++i) {
			localMatrix[i] = MakeSRTMatrix(scales[i], rotates[i], translates[i]);
		}

		// ワールド行列を作成
		Matrix4x4 worldMatrix[3] = {
			localMatrix[0],
			localMatrix[1] * localMatrix[0],
			localMatrix[2] * localMatrix[1] * localMatrix[0]
		};

		// ワールド座標系の肩、肘、手の球を作成
		Sphere worldSpheres[3];
		for (uint32_t i = 0; i < 3; ++i) {
			worldSpheres[i] = localSpheres[i];
			worldSpheres[i].center = localSpheres[i].center * worldMatrix[i];
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

		// 肩、肘、手の球のスクリーン座標を作成
		Vector3 screenPositions[3];
		for (uint32_t i = 0; i < 3; ++i) {
			screenPositions[i] = worldSpheres[i].center * viewProjectionMatrix * viewportMatrix;
		}

		// 肩ー肘の線を描画
		Novice::DrawLine(
			static_cast<int32_t>(screenPositions[0].x),
			static_cast<int32_t>(screenPositions[0].y),
			static_cast<int32_t>(screenPositions[1].x),
			static_cast<int32_t>(screenPositions[1].y),
			WHITE
		);

		// 肘ー手の線を描画
		Novice::DrawLine(
			static_cast<int32_t>(screenPositions[1].x),
			static_cast<int32_t>(screenPositions[1].y),
			static_cast<int32_t>(screenPositions[2].x),
			static_cast<int32_t>(screenPositions[2].y),
			WHITE
		);

		// 肩、肘、手の球を描画
		for (uint32_t i = 0; i < 3; ++i) {
			DrawSphere(worldSpheres[i], viewProjectionMatrix, viewportMatrix, colors[i]);
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
