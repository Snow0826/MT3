#include <Novice.h>
#include "Matrix4x4.h"
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

	// マウスの位置を受け取る箱
	int preMouseX = 0;
	int preMouseY = 0;
	int mouseX = 0;
	int mouseY = 0;

	// カメラの初期化
	Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };

	// 球の初期化
	Sphere sphere;
	sphere = { { 0.0f, 0.0f, 3.0f }, 1.0f };

	// 平面の初期化
	Plane plane;
	plane = { { 0.0f, 1.0f, 0.0f }, 1.5f };

	// 色の初期化
	uint32_t sphereColor = WHITE;
	uint32_t planeColor = WHITE;

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

		// マウスの前フレームの位置を更新
		preMouseX = mouseX;
		preMouseY = mouseY;

		if (keys[DIK_LSHIFT] && !preKeys[DIK_LSHIFT] || Novice::IsTriggerMouse(2)) {
			Novice::GetMousePosition(&preMouseX, &preMouseY);	// マウスの前フレームの位置を取得
		}

		// カメラの操作
		if (Novice::IsPressMouse(2)) {
			Novice::GetMousePosition(&mouseX, &mouseY);	// マウスの現在フレームの位置を取得
			if (keys[DIK_LSHIFT]) {
				cameraTranslate.x -= (mouseX - preMouseX) * 0.01f;
				cameraTranslate.y += (mouseY - preMouseY) * 0.01f;
			} else {
				cameraRotate.y -= (mouseX - preMouseX) * 0.01f;
				cameraRotate.x += (mouseY - preMouseY) * 0.01f;
			}
		}

		if (Novice::GetWheel() > 0) {
			cameraTranslate.z += 0.1f;
		} else if(Novice::GetWheel() < 0) {
			cameraTranslate.z -= 0.1f;
		}

		// ImGuiの設定
		ImGui::Begin("Debug");
		
		if (ImGui::TreeNode("sphere")) {
			ImGui::DragFloat3("center", &sphere.center.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat("radius", &sphere.radius, 0.01f, 0.0f, 10.0f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("plane")) {
			ImGui::DragFloat3("normal", &plane.normal.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat("distance", &plane.distance, 0.01f, -10.0f, 10.0f);
			ImGui::TreePop();
		}

		ImGui::End();

		plane.normal = plane.normal.normalized();	// 法線ベクトルを正規化

		// 球の衝突判定
		if (isCollision(sphere, plane)) {
			sphereColor = RED;
		} else {
			sphereColor = WHITE;
		}

		// レンダリングパイプライン
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = cameraMatrix.inverse();
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = viewMatrix * projectionMatrix;
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
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, sphereColor);

		// 平面を描画
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, planeColor);

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
