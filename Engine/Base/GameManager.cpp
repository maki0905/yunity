#include "GameManager.h"


#include "Device.h"
#include "Input.h"

GameManager::GameManager()
{
	Framework::Initialize();
	
}

GameManager::~GameManager()
{
	//Framework::Finalize();
}

void GameManager::Initialize()
{
	CameraManager::GetInstance()->Initialize();
}

void GameManager::Finalize()
{
	CameraManager::GetInstance()->Finalize();
	Framework::Finalize();
}

void GameManager::Update()
{
	CameraManager::GetInstance()->Update();
}

void GameManager::Draw()
{
	DirectXCore::GetInstance()->PreDrawRenderTexture();
#pragma region 背景描画
	sprite_->PreDraw(DirectXCore::GetInstance()->GetCommandList());
	SceneManager::GetInstance()->DrawBack();
	sprite_->PostDraw();
#pragma endregion 

#pragma region 3D描画
	skybox_->PreDraw(DirectXCore::GetInstance()->GetCommandList());
	model_->PreDraw(DirectXCore::GetInstance()->GetCommandList());
	primitive_->PreDraw(DirectXCore::GetInstance()->GetCommandList());
	// 描画
	SceneManager::GetInstance()->Draw3D();
	primitive_->PostDraw();
	model_->PostDraw();
	skybox_->PostDraw();
#pragma endregion

#pragma region 前景描画

	/*sprite_->PreDraw(DirectXCore::GetInstance()->GetCommandList());
	SceneManager::GetInstance()->DrawFront();
	sprite_->PostDraw();*/

	DirectXCore::GetInstance()->PostDrawRenderTexture();
#pragma endregion

	DirectXCore::GetInstance()->PreDrawSwapchain();

	sprite_->PreDraw(DirectXCore::GetInstance()->GetCommandList());
	SceneManager::GetInstance()->DrawFront();
	sprite_->PostDraw();

	// ImGui描画
	ImGuiManager::GetInstance()->Draw();

	DirectXCore::GetInstance()->PostDrawSwapchain();
}

void GameManager::Run()
{
	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		if (WindowsAPI::GetInstance()->ProcessMessage()) {
			break;
		}

		Framework::Update();
		Update();
		Draw();
		
	}
	//imguiManager_->Finalize();
}


