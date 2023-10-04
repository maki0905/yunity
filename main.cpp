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

#include "T.h"
#include "ViewProjection.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	GameScene* gameScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// テクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	Triangle::StaticInitialize(dxCommon->GetDevice());

	gameScene = new GameScene();
	gameScene->Initialize(dxCommon);
	ViewProjection viewProjection_;
	viewProjection_.Initialize();
	T* t = new T();
	t->Initialize();
	t->GetTexture(TextureManager::Load("uvChecker.png"));
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
			
			// ImGui受付開始
			imguiManager->Begin();

			//triangle->Update();
			t->Update();
			viewProjection_.UpdateMatrix();

			// ImGui受付終了
			imguiManager->End();

			// 描画開始
			dxCommon->PreDraw();

			Triangle::PreDraw(dxCommon->GetCommandList());

			t->Draw(viewProjection_);

			Triangle::PostDraw();

			/*triangle->PreDraw(dxCommon->GetCommandList());
			triangle->Draw();
			triangle->PostDraw();*/

			// ImGui描画
			imguiManager->Draw();

			// 描画終了
			dxCommon->PostDraw();

		}
	}

	//SafeDelete(gameScene);
	//SafeDelete(dxCommon);
	
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
	//SafeDelete(win);
	return 0;
}