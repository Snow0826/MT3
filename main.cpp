#include <Novice.h>
#include <imgui.h>
#include "Camera.h"
#include "Draw.h"
#include "Physics.h"
#include "Collision.h"

constexpr char kWindowTitle[] = "LE2A_03_クラタ_ユウキ_MT3_04_04";
constexpr int32_t kWindowWidth = 1280;	// ウィンドウの幅
constexpr int32_t kWindowHeight = 720;	// ウィンドウの高さ

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// カメラの初期化
	Camera camera;
	camera.Initialize();

	// 平面の初期化
	Plane plane{};
	plane.normal = Vector3{ -0.2f, 1.1f, -0.3f }.normalized();
	plane.distance = 0.0f;

	// ボールの初期化
	Ball ball{};
	ball.position = { 0.8f, 1.2f, 0.3f };
	ball.acceleration = { 0.0f, -9.8f, 0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = WHITE;

	// カプセルの初期化
	Capsule capsule{};
	capsule.radius = ball.radius;

	float deltaTime = 1.0f / 60.0f; // 1フレームあたりの時間（秒）
	bool isStarted = false; // シミュレーションが開始されたかどうか

	constexpr float e = 0.8f; // 反発係数
	constexpr float kEpsilon = 0.0001f; // 衝突判定のための小さな値
	Vector3 reflected;	// 反射ベクトル
	Vector3 projectNormal; // 法線方向の射影ベクトル
	Vector3 movingDirection; // 移動方向

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
		ImGui::DragFloat3("BallPosition", &ball.position.x, 0.01f, -1.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp); // ボールの位置を調整
		ImGui::DragFloat3("BallVelocity", &ball.velocity.x, 0.01f, -10.0f, 10.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp); // ボールの速度を調整
		ImGui::DragFloat3("PlaneNormal", &plane.normal.x, 0.01f, -1.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp); // 平面の法線ベクトルを調整
		ImGui::DragFloat3("Reflected", &reflected.x, 0.01f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);	// 反射ベクトルを調整
		ImGui::DragFloat3("ProjectNormal", &projectNormal.x, 0.01f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp); // 法線方向の射影ベクトルを調整
		ImGui::DragFloat3("MovingDirection", &movingDirection.x, 0.01f, -1.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp); // 移動方向を調整
		plane.normal = plane.normal.normalized(); // 法線ベクトルを正規化
		if (ImGui::Button("Reset")) {
			isStarted = false; // シミュレーションをリセット
			ball.position = { 0.8f, 1.2f, 0.3f }; // ボールの位置を初期化
			ball.velocity = { 0.0f, 0.0f, 0.0f }; // ボールの速度を初期化
			ball.color = WHITE; // ボールの色を初期化
			capsule.segment.origin = ball.position; // カプセルの始点をボールの位置に設定
			capsule.segment.diff = { 0.0f, 0.0f, 0.0f }; // カプセルの差分ベクトルを初期化
			plane.normal = Vector3{ -0.2f, 1.1f, -0.3f }.normalized();
		}
		ImGui::End();

		if (isStarted) {
			capsule.segment.origin = ball.position; // カプセルの始点を前フレームのボールの位置に設定
			ball.velocity += ball.acceleration * deltaTime; // 速度の更新
			ball.position += ball.velocity * deltaTime; // 位置の更新
			capsule.segment.diff = ball.position - capsule.segment.origin; // ボールの位置の現在フレームと前フレームの差分ベクトルを計算

			if (isCollision(capsule, plane)) {
				reflected = Reflect(ball.velocity, plane.normal); // 反射ベクトルの計算
				projectNormal = Project(reflected, plane.normal); // 法線方向の投影
				movingDirection = reflected - projectNormal; // 移動方向の計算
				ball.velocity = projectNormal * e + movingDirection; // 衝突後のボールの速度を更新
				ball.position = capsule.segment.origin + kEpsilon * plane.normal;	// 衝突後のボールの位置を更新
			}
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

		// 平面を描画
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);

		// 球を描画
		DrawSphere(Sphere{ ball.position, ball.radius }, viewProjectionMatrix, viewportMatrix, ball.color);

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