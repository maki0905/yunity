#include "GameManager.h"


#include "Device.h"
#include "Input.h"

GameManager::GameManager()
{
	Framework::Initialize();
	
}

GameManager::~GameManager()
{
	Framework::Finalize();
	//Framework::Finalize();
}

void GameManager::Initialize()
{
}

void GameManager::Finalize()
{
}

void GameManager::Update()
{
}

void GameManager::Draw()
{
	directXCore_->PreDraw();
#pragma region 背景描画
	sprite_->PreDraw(directXCore_->GetCommandList());
	sceneArr_[currentSceneNo_]->DrawBack();
	sprite_->PostDraw();
#pragma endregion 

#pragma region 3D描画
	model_->PreDraw(directXCore_->GetCommandList());
	sphere_->PreDraw(directXCore_->GetCommandList());
	particle_->PreDraw(directXCore_->GetCommandList());
	primitive_->PreDraw(directXCore_->GetCommandList());
	// 描画
	sceneArr_[currentSceneNo_]->Draw3D();
	primitive_->PostDraw();
	particle_->PostDraw();
	sphere_->PostDraw();
	model_->PostDraw();
#pragma endregion

#pragma region 前景描画

	sprite_->PreDraw(directXCore_->GetCommandList());
	sceneArr_[currentSceneNo_]->DrawFront();
	sprite_->PostDraw();

#pragma endregion

	// ImGui描画
	imguiManager_->Draw();

	directXCore_->PostDraw();
}

void GameManager::Run()
{
	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		if (windowsAPI_->ProcessMessage()) {
			break;
		}

		Framework::Update();
		Draw();
		
	}
	//imguiManager_->Finalize();
}


