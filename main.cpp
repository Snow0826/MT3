#include <Novice.h>
#include <imgui.h>
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
	ball.position = { 0.0f, 0.0f, 0.0f };
	ball.mass = 1.0f;
	ball.radius = 0.05f;
	ball.color = WHITE;

	// 振り子の初期化
	Pendulum pendulum{};
	pendulum.anchor = { 0.0f, 1.0f, 0.0f };
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;

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
			pendulum.angularAcceleration = -9.8f / pendulum.length * std::sin(pendulum.angle); // 振り子の角加速度を計算
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime; // 角速度を更新
			pendulum.angle += pendulum.angularVelocity * deltaTime; // 角度を更新
		}

		// ボールの位置を更新
		ball.position.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
		ball.position.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
		ball.position.z = pendulum.anchor.z; // Z座標は固定

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

		// アンカーから振り子の先端までの線を描画
		Vector3 start = pendulum.anchor * viewProjectionMatrix * viewportMatrix;
		Vector3 end = ball.position * viewProjectionMatrix * viewportMatrix;
		Novice::DrawLine(
			static_cast<int32_t>(start.x),
			static_cast<int32_t>(start.y),
			static_cast<int32_t>(end.x),
			static_cast<int32_t>(end.y),
			WHITE
		);

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
