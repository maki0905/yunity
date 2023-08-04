#include <Windows.h>
#include "WinApp.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "Model.h"
#include "Vector4.h"
#include "CreateBufferResource.h"
#include "WorldTransform.h"
#include "GameScene.h"
#include "Triangle.h"
#include <dxcapi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	GameScene* gameScene = nullptr;
	Triangle* triangle = nullptr;
	triangle = new Triangle();
	

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

	//TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	//TextureManager::Load("Resources/uvChecker.png");
	/*HRESULT result_ = S_FALSE;
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	result_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(result_));
	result_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(result_));*/

	triangle->StaticInitialize(dxCommon->GetDevice());
	
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon);
	
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
			dxCommon->PreDraw();

			triangle->PreDraw(dxCommon->GetCommandList());
			triangle->Draw();
			triangle->PostDraw();


			dxCommon->PostDraw();

		}
	}

	//SafeDelete(gameScene);
	//SafeDelete(dxCommon);
	

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
	//SafeDelete(win);
	return 0;
}