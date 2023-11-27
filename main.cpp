#include <Windows.h>
#include "WinApp.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Model.h"
#include "Vector4.h"
#include "CreateBufferResource.h"
#include "WorldTransform.h"
#include "GameScene.h"
#include "Triangle.h"
#include <dxcapi.h>
#include "Input.h"

#include "ViewProjection.h"
#include "GlobalVariables.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	GameScene* gameScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// テクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	Sprite::StaticInitialize(dxCommon->GetDevice());

	Model::StaticInitialize(dxCommon->GetDevice());

#pragma endregion

	GlobalVariables::GetInstance()->LoadFiles();
	
	MSG msg{};

	gameScene = new GameScene();
	gameScene->Initialize();
	// ウィンドウのxボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		// Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			
			// ImGui受付開始
			imguiManager->Begin();

			// 入力関連の毎フレーム処理
			input->Update();

#ifdef _DEBUG
			GlobalVariables::GetInstance()->Updata();
#endif // _DEBUG

			// ゲームシーンの毎フレーム処理
			gameScene->Update();

			// ImGui受付終了
			imguiManager->End();

			// 描画開始
			dxCommon->PreDraw();

			// ゲームシーンの描画
			gameScene->Draw();

			// ImGui描画
			imguiManager->Draw();

			// 描画終了
			dxCommon->PostDraw();

		}
	}

	SafeDelete(gameScene);
	//SafeDelete(input);
	//SafeDelete(dxCommon);
	
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
	//SafeDelete(win);
	return 0;
}