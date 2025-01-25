#include "SceneManager.h"

#include <cassert>
#include "ModelManager.h"

yunity::SceneManager* yunity::SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

void yunity::SceneManager::Update()
{
	// 次のシーンの予約があるなら
	if (nextScene_) {
		// 旧シーンの終了
		if (scene_) {
			scene_.reset();
		}

		ModelManager::GetInstance()->Reset();

		// シーン切り替え
		scene_.reset(nextScene_);
		nextScene_ = nullptr;
		// 次のシーンを初期化
		scene_->Initialize();
	}


	// 実行中シーンを更新する
	scene_->Update();

}

void yunity::SceneManager::DrawBack()
{
	scene_->DrawBack();
}

void yunity::SceneManager::Draw3D()
{
	scene_->Draw3D();
}

void yunity::SceneManager::DrawFront()
{
	scene_->DrawFront();
}

void yunity::SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(sceneName).release();
}


