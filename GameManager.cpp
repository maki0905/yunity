#include "GameManager.h"

#include "Device.h"
#include "Input.h"

GameManager::GameManager()
{

	// 各シーンの配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[GAME_STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();

	// 初期シーンの設定
	currentSceneNo_ = GAME_STAGE;

	// WindowsAPI
	windowsAPI_ = WindowsAPI::GetInstance();
	windowsAPI_->CreateGameWindow();

	// DirectX基盤
	directXCore_ = DirectXCore::GetInstance();
	directXCore_->Initialize();

#pragma region 汎用機能初期化

	// Input
	Input* input = Input::GetInstance();
	input->Initialize();

	// 3Dオブジェクト
	t_ = new T();
	t_->StaticInitialize();


#ifdef _DEBUG
	// ImGui
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize();
#endif // _DEBUG

#pragma endregion

}

GameManager::~GameManager()
{
	windowsAPI_->TerminateGameWindow();
}

void GameManager::Run()
{
	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		if (windowsAPI_->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager_->Begin();

		// シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		// シーン変更チェック
		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Initialize();
		}

		// 更新
		sceneArr_[currentSceneNo_]->Update();

		// ImGui受付終了
		imguiManager_->End();

		directXCore_->PreDraw();

		t_->PreDraw(directXCore_->GetCommandList());

		// 描画
		sceneArr_[currentSceneNo_]->Draw();

		t_->PostDraw();

		// ImGui描画
		imguiManager_->Draw();

		directXCore_->PostDraw();

	}
	imguiManager_->Finalize();
}
