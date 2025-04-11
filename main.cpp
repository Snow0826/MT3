#include <Novice.h>
#include "Matrix4x4.h"

const char kWindowTitle[] = "LE2A_03_クラタ_ユウキ_MT3_00_02";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 行列の初期化
	Matrix4x4 m1 = {
		3.2f, 0.7f, 9.6f, 4.4f,
		5.5f, 1.3f, 7.8f, 2.1f,
		6.9f, 8.0f, 2.6f, 1.0f,
		0.5f, 7.2f, 5.1f, 3.3f
	};

	Matrix4x4 m2 = {
		4.1f, 6.5f, 3.3f, 2.2f,
		8.8f, 0.6f, 9.9f, 7.7f,
		1.1f, 5.5f, 6.6f, 0.0f,
		3.3f, 9.9f, 8.8f, 2.2f
	};

	const int kRowHeight = 20;
	const int kColumnWidth = 60;

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

		// 行列の演算
		Matrix4x4 resultAdd = m1 + m2;
		Matrix4x4 resultSubtract = m1 - m2;
		Matrix4x4 resultMultiply = m1 * m2;
		Matrix4x4 inverseM1 = m1.inverse();
		Matrix4x4 inverseM2 = m2.inverse();
		Matrix4x4 transposeM1 = m1.transpose();
		Matrix4x4 transposeM2 = m2.transpose();
		Matrix4x4 identity = MakeIdentity4x4();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 行列の演算結果の表示
		resultAdd.ScreenPrintf(0, 0, "Add");
		resultSubtract.ScreenPrintf(0, kRowHeight * 5, "Subtract");
		resultMultiply.ScreenPrintf(0, kRowHeight * 10, "Multiply");
		inverseM1.ScreenPrintf(0, kRowHeight * 15, "inverseM1");
		inverseM2.ScreenPrintf(0, kRowHeight * 20, "inverseM2");
		transposeM1.ScreenPrintf(kColumnWidth * 5, 0, "transposeM1");
		transposeM2.ScreenPrintf(kColumnWidth * 5, kRowHeight * 5, "transposeM2");
		identity.ScreenPrintf(kColumnWidth * 5, kRowHeight * 10, "identity");

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
