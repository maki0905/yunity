#include "GameManager.h"

GameManager::GameManager()
{

	// 各シーンの配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[GAME_STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();

	// 初期シーンの設定
	currentSceneNo_ = GAME_STAGE;

	//directXCore_ = DirectXCore::GetInstance();
}

GameManager::~GameManager()
{
}

void GameManager::Run()
{
	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		// シーン変更チェック
		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Initialize();
		}

		// 更新
		sceneArr_[currentSceneNo_]->Update();

		//directXCore_->PreDraw();

		// 描画
		sceneArr_[currentSceneNo_]->Draw();

		//directXCore_->PostDraw();

	}
}
