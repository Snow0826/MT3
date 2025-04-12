#include <Novice.h>
#include "Matrix4x4.h"

const char kWindowTitle[] = "LE2A_03_クラタ_ユウキ_MT3_01_00";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 初期化
	Vector3 v1{ 1.2f, -3.9f, 2.5f };
	Vector3 v2{ 2.8f, 0.4f, -1.3f };
	Vector3 rotate{};
	Vector3 translate{};
	const Vector3 kLocalVertices[3] = {
		{ 0.0f, 0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{ 0.5f, -0.5f, 0.0f }
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

		// 回転と移動
		rotate.y += 0.03f;

		if (keys[DIK_W]) {
			translate.z += 0.1f;
		}

		if (keys[DIK_S]) {
			translate.z -= 0.1f;
		}

		if (keys[DIK_A]) {
			translate.x -= 0.1f;
		}

		if (keys[DIK_D]) {
			translate.x += 0.1f;
		}

		// 外積
		Vector3 cross = v1.cross(v2);

		// 描画座標の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -5.0f });

		Matrix4x4 viewMatrix = cameraMatrix.inverse();
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);

		Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);

		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = kLocalVertices[i] * worldViewProjectionMatrix;
			screenVertices[i] = ndcVertex * viewportMatrix;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 外積の結果を表示
		cross.ScreenPrintf(0, 0, "Cross");

		// 三角形を描画
		Novice::DrawTriangle(
			static_cast<int>(screenVertices[0].x),
			static_cast<int>(screenVertices[0].y),
			static_cast<int>(screenVertices[1].x),
			static_cast<int>(screenVertices[1].y),
			static_cast<int>(screenVertices[2].x),
			static_cast<int>(screenVertices[2].y),
			RED, kFillModeSolid
		);

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
