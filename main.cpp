#include <Novice.h>
#include "Matrix4x4.h"
#include "Camera.h"
#include "Draw.h"
#include "Collision.h"
#include <imgui.h>

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

	// 三角形の初期化
	Triangle triangle;
	triangle.vertices[0] = { 0.0f, 1.0f, 0.0f };
	triangle.vertices[1] = { 1.0f, 1.0f, 0.0f };
	triangle.vertices[2] = { 0.0f, 1.0f, 1.0f };

	// 線分の初期化
	Segment segment;
	segment.origin = { 0.0f, 0.0f, 0.0f };
	segment.diff = { 0.0f, 0.5f, 0.0f };

	// 色の初期化
	uint32_t segmentColor = WHITE;
	uint32_t triangleColor = WHITE;

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

		if (ImGui::TreeNode("triangle")) {
			ImGui::DragFloat3("vertex0", &triangle.vertices[0].x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat3("vertex1", &triangle.vertices[1].x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat3("vertex2", &triangle.vertices[2].x, 0.01f, -10.0f, 10.0f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("segment")) {
			ImGui::DragFloat3("origin", &segment.origin.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat3("diff", &segment.diff.x, 0.01f, 0.0f, 10.0f);
			ImGui::TreePop();
		}

		ImGui::End();

		// 三角形と線分の衝突判定
		if (isCollision(triangle, segment)) {
			segmentColor = RED;
		} else {
			segmentColor = WHITE;
		}

		// レンダリングパイプライン
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = camera.GetViewMatrix() * projectionMatrix;
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f); 
		Vector3 start = segment.origin * viewProjectionMatrix * viewportMatrix;
		Vector3 end = (segment.origin + segment.diff) * viewProjectionMatrix * viewportMatrix;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		// 三角形を描画
		DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, triangleColor);

		// 線分を描画
		Novice::DrawLine(
			static_cast<int32_t>(start.x),
			static_cast<int32_t>(start.y),
			static_cast<int32_t>(end.x),
			static_cast<int32_t>(end.y),
			segmentColor);

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
