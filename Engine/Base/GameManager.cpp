#include "GameManager.h"


#include "Device.h"
#include "Input.h"
#include "CommonData.h"

void GameManager::Initialize()
{
	Framework::Initialize();
	sceneFactory_ = std::make_unique<yunity::SceneFactory>();
	yunity::SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
	yunity::SceneManager::GetInstance()->ChangeScene("TITLE");
	CameraManager::GetInstance()->Initialize();
	CommonData::GetInstance()->Initialize();
}

void GameManager::Finalize()
{
	Framework::Finalize();
}

void GameManager::Update()
{
	Framework::Update();
	CameraManager::GetInstance()->Update();
	if (yunity::WindowsAPI::GetInstance()->ProcessMessage()) {
		endRequest_ = true;
	}
}

void GameManager::Draw()
{
#pragma region シャドウ描画
	yunity::DirectXCore::GetInstance()->PreDrawShadow();
	skybox_->PreDraw(yunity::DirectXCore::GetInstance()->GetCommandList(), yunity::PipelineType::kShadowMap);
	model_->PreDraw(yunity::DirectXCore::GetInstance()->GetCommandList(), yunity::PipelineType::kShadowMap);
	// 描画
	yunity::SceneManager::GetInstance()->Draw3D();
	model_->PostDraw();
	skybox_->PostDraw();
	yunity::DirectXCore::GetInstance()->PostDrawShadow();
#pragma endregion

	yunity::DirectXCore::GetInstance()->PreDrawRenderTexture();
#pragma region 背景描画
	sprite_->PreDraw(yunity::DirectXCore::GetInstance()->GetCommandList());
	yunity::SceneManager::GetInstance()->DrawBack();
	sprite_->PostDraw();
#pragma endregion 

#pragma region 3D描画
	// 通常描画用
	skybox_->PreDraw(yunity::DirectXCore::GetInstance()->GetCommandList());
	model_->PreDraw(yunity::DirectXCore::GetInstance()->GetCommandList());
	particleDrawer_->PreDraw(yunity::DirectXCore::GetInstance()->GetCommandList());
	primitive_->PreDraw(yunity::DirectXCore::GetInstance()->GetCommandList());
	// 描画
	yunity::SceneManager::GetInstance()->Draw3D();
	primitive_->PostDraw();
	particleDrawer_->PostDraw();
	model_->PostDraw();
	skybox_->PostDraw();
#pragma endregion

#pragma region 前景描画

	yunity::DirectXCore::GetInstance()->PostDrawRenderTexture();

	yunity::DirectXCore::GetInstance()->PreDrawSwapchain();

	sprite_->PreDraw(yunity::DirectXCore::GetInstance()->GetCommandList());
	yunity::SceneManager::GetInstance()->DrawFront();
	sprite_->PostDraw();
#pragma endregion

	// ImGui描画
	yunity::ImGuiManager::GetInstance()->Draw();

	yunity::DirectXCore::GetInstance()->PostDrawSwapchain();
}


