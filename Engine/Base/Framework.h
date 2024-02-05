#pragma once

#include <memory>

#pragma region シーン
#include "IScene.h"
#include "TitleScene.h"
#include "StageScene.h"
#include "ClearScene.h"
#pragma endregion 

#include "DirectXCore.h"
#include "WindowsAPI.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Model.h"
#include "Sprite.h"
#include "PrimitiveDrawer.h"
#include "ParticleDrawer.h"
#include "SphereDrawer.h"
#include "Audio/Audio.h"
#include "SceneManager.h"

class Framework {
public:
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update();
	virtual void Draw() = 0;

protected:
	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> sceneArr_[COUNT];

	// シーン管理用の変数
	int currentSceneNo_; // 現在のシーン
	int prevSceneNo_; // 前のシーン

	DirectXCore* directXCore_ = nullptr;
	WindowsAPI* windowsAPI_ = nullptr;
	Input* input_ = nullptr;
	ImGuiManager* imguiManager_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Model* model_ = nullptr;
	Sprite* sprite_ = nullptr;
	PrimitiveDrawer* primitive_ = nullptr;
	ParticleDrawer* particle_ = nullptr;
	SphereDrawer* sphere_ = nullptr;
	Audio* audio_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
};