#include <Novice.h>
#include "Matrix4x4.h"

const char kWindowTitle[] = "LE2A_03_クラタ_ユウキ_MT3_00_04";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate{ 0.4f, 1.43f, -0.8f };	// 回転角度

	const int kRowHeight = 20;

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

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		Matrix4x4 rotateXYZMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		rotateXMatrix.ScreenPrintf(0, 0, "rotateXMatrix");
		rotateYMatrix.ScreenPrintf(0, kRowHeight * 5, "rotateYMatrix");
		rotateZMatrix.ScreenPrintf(0, kRowHeight * 10, "rotateZMatrix");
		rotateXYZMatrix.ScreenPrintf(0, kRowHeight * 15, "rotateXYZMatrix");

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
