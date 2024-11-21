#include "Framework.h"

void yunity::Framework::Initialize()
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

	// モデルマネージャー
	ModelManager::GetInstance()->Initialize();

	// モデル
	model_ = std::make_unique<Model>();

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->StaticInitialize();

	// 基本形状
	primitive_ = std::make_unique<PrimitiveDrawer>();
	primitive_->StaticInitialize();

	// テクスチャマネージャー
	TextureManager::GetInstance()->Initialize();
	TextureManager::Load("white1x1.png");

	// オーディオ
	Audio::GetInstance()->Initialize();

	// シーンマネージャー
	SceneManager::GetInstance()->Initialize();

	// CSV
	CSV::GetInstance()->Initialize();

	// Skybox
	skybox_ = std::make_unique<SkyBox>();
	skybox_->StaticInitialize();

	// パーティクル
	particleDrawer_ = std::make_unique<ParticleDrawer>();
	particleDrawer_->StaticInitialize();

#pragma endregion

}

void yunity::Framework::Finalize()
{
	skybox_->Finalize();
	primitive_->Finalize();
	sprite_->Finalize();
	ImGuiManager::GetInstance()->Finalize();
	WindowsAPI::GetInstance()->TerminateGameWindow();
}

void yunity::Framework::Update()
{
	// ImGui受付開始
	ImGuiManager::GetInstance()->Begin();

	Input::GetInstance()->Update();

	SceneManager::GetInstance()->Update();
	ModelManager::GetInstance()->Update();

	// ImGui受付終了
	ImGuiManager::GetInstance()->End();
}

void yunity::Framework::Run()
{
	// ゲームの初期化
	Initialize();

	// ゲームループ
	while (true) {
		// 毎フレーム更新
		Update();
		if (IsEndRequest()) {
			break;
		}
		// 描画
		Draw();
	}
	Finalize();
}
