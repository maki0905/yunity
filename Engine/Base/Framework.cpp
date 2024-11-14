#include "Framework.h"

void Framework::Initialize()
{
	// WindowsAPI
	WindowsAPI::GetInstance()->CreateGameWindow();

	// DirectX基盤
	DirectXCore::GetInstance()->Initialize();

#pragma region 汎用機能初期化

#ifdef _DEBUG
	// ImGui
	ImGuiManager::GetInstance()->Initialize();
#endif // _DEBUG

	// Input
	Input::GetInstance()->Initialize();


	ModelManager::GetInstance()->Initialize();

	model_ = std::make_unique<Model>();

	sprite_ = std::make_unique<Sprite>();
	sprite_->StaticInitialize();

	primitive_ = std::make_unique<PrimitiveDrawer>();
	primitive_->StaticInitialize();

	// テクスチャマネージャー
	TextureManager::GetInstance()->Initialize();
	TextureManager::Load("white1x1.png");

	Audio::GetInstance()->Initialize();

	SceneManager::GetInstance()->Initialize();
	SceneManager::GetInstance()->ChangeScene("TITLE");

	CSV::GetInstance()->Initialize();

	skybox_ = std::make_unique<SkyBox>();
	skybox_->StaticInitialize();

#pragma endregion

}

void Framework::Finalize()
{
	skybox_->Finalize();
	primitive_->Finalize();
	sprite_->Finalize();
	ImGuiManager::GetInstance()->Finalize();
	WindowsAPI::GetInstance()->TerminateGameWindow();
}

void Framework::Update()
{
	// ImGui受付開始
	ImGuiManager::GetInstance()->Begin();

	Input::GetInstance()->Update();

	SceneManager::GetInstance()->Update();
	ModelManager::GetInstance()->Update();

	// ImGui受付終了
	ImGuiManager::GetInstance()->End();
}
