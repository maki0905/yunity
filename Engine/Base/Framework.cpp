#include "Framework.h"

#include "GlobalVariables.h"

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

	// テクスチャマネージャー
	TextureManager::GetInstance()->Initialize();
	TextureManager::Load("white1x1.png");

	// オーディオ
	Audio::GetInstance()->Initialize();

	// シーンマネージャー
	sceneFactory_ = std::make_unique<SceneFactory>();
	SceneManager::GetInstance()->Initialize(sceneFactory_.get());

	// CSV
	CSV::GetInstance()->Initialize();

	// Skybox
	skybox_ = std::make_unique<SkyBox>();

	// パーティクル
	particleDrawer_ = std::make_unique<ParticleDrawer>();

	GlobalVariables::GetInstance()->LoadFiles();

#pragma endregion

}

void yunity::Framework::Finalize()
{
	ImGuiManager::GetInstance()->Finalize();
	WindowsAPI::GetInstance()->TerminateGameWindow();
}

void yunity::Framework::Update()
{
	// ImGui受付開始
	ImGuiManager::GetInstance()->Begin();

	Input::GetInstance()->Update();

#ifdef _DEBUG
	GlobalVariables::GetInstance()->Updata();
#endif 

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
