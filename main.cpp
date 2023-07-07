#include <Windows.h>
#include "WinApp.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	MSG msg{};
	// ウィンドウのxボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		// Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ゲーム処理
		}
	}
	OutputDebugStringA("Hello,DirectX!\n");
	return 0;
}