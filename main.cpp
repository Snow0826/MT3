#include <Novice.h>
#include <imgui.h>
#include <numbers>
#include "Camera.h"
#include "Draw.h"
#include "Physics.h"

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

	// ボールの初期化
	Ball ball{};
	ball.position = { 0.8f, 0.0f, 0.0f };
	ball.mass = 1.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

	Vector3 center;	// 円の中心座標
	float radius = 0.8f; // 円の半径
	float angularVelocity = std::numbers::pi_v<float>; // ボールの角速度
	float angle = 0.0f; // ボールの角度
	float deltaTime = 1.0f / 60.0f; // 1フレームあたりの時間（秒）
	bool isStarted = false; // シミュレーションが開始されたかどうか

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
		ImGui::Begin("Window");
		if (ImGui::Button("Start")) {
			isStarted = true; // シミュレーションを開始
		}
		ImGui::End();
		
		if (isStarted) {
			angle += angularVelocity * deltaTime; // ボールの角度を更新
			ball.position.x = center.x + std::cos(angle) * radius; // ボールの位置を更新
			ball.position.y = center.y + std::sin(angle) * radius; // ボールの位置を更新
			ball.position.z = center.z; // Z座標は円の中心のZ座標に固定
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

		// 球を描画
		Sphere sphere{};
		sphere.center = ball.position;
		sphere.radius = ball.radius;
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, ball.color);

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
