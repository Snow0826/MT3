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

	// ばねの初期化
	Spring spring{};
	spring.anchor = { 0.0f, 0.0f, 0.0f };
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	// ボールの初期化
	Ball ball{};
	ball.position = { 1.2f, 0.0f, 0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

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
			Vector3 diff = ball.position - spring.anchor; // ボールとばねのアンカーの差分を計算
			float length = diff.length(); // 差分の長さを計算
			if (length != 0.0f) {
				Vector3 direction = diff.normalized(); // 差分の方向を正規化
				Vector3 restPosition = spring.anchor + direction * spring.naturalLength; // ばねの自然長に基づく位置を計算
				Vector3 displacement = length * (ball.position - restPosition); // ボールの位置とばねの自然長に基づく位置の差分を計算
				Vector3 restoringForce = -spring.stiffness * displacement; // ばねの復元力を計算
				Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity; // 減衰力を計算
				Vector3 force = restoringForce + dampingForce; // 力の合計を計算
				ball.acceleration = force / ball.mass; // ボールの加速度を計算
			}

			ball.velocity += ball.acceleration * deltaTime; // ボールの速度を更新
			ball.position += ball.velocity * deltaTime; // ボールの位置を更新
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

		// アンカーから球までの線を描画
		Vector3 start = spring.anchor * viewProjectionMatrix * viewportMatrix; // ばねのアンカー位置
		Vector3 end = ball.position * viewProjectionMatrix * viewportMatrix; // ボールの位置を変換
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
