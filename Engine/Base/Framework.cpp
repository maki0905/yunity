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

	/*particle_ = new ParticleDrawer();
	particle_->StaticInitialize();*/

	// テクスチャマネージャー
	TextureManager::GetInstance()->Initialize();
	TextureManager::Load("white1x1.png");

	///*sphere_ = new SphereDrawer();
	//sphere_->StaticInitialize();*/

	Audio::GetInstance()->Initialize();

	SceneManager::GetInstance()->Initialize();
	SceneManager::GetInstance()->ChangeScene("TITLE");

	CSV::GetInstance()->Initialize();

	skybox_ = std::make_unique<SkyBox>();
	skybox_->StaticInitialize();

	ObjectManager::GetInstance()->Initialize();

#pragma endregion

	// 各シーンの配列
	/*sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[GAME_STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();*/

	// 初期シーンの設定
	/*currentSceneNo_ = GAME_STAGE;
	sceneArr_[currentSceneNo_]->Initialize();*/
}

void Framework::Finalize()
{
	skybox_->Finalize();
	primitive_->Finalize();
	sprite_->Finalize();
	model_->Finalize();
	ModelManager::GetInstance()->Finalize();
	ImGuiManager::GetInstance()->Finalize();
	WindowsAPI::GetInstance()->TerminateGameWindow();
	/*imguiManager_->Finalize();
	skybox_->Finalize();
	primitive_->Finalize();
	sprite_->Finalize();
	directXCore_->Finalize();
	windowsAPI_->TerminateGameWindow();*/
}

void Framework::Update()
{
	// ImGui受付開始
	ImGuiManager::GetInstance()->Begin();

	Input::GetInstance()->Update();

	// シーンのチェック
	/*prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();*/

	// シーン変更チェック
	/*if (prevSceneNo_ != currentSceneNo_) {
		sceneArr_[currentSceneNo_]->Initialize();
	}*/

	// 更新
	//sceneArr_[currentSceneNo_]->Update();

	SceneManager::GetInstance()->Update();
	ModelManager::GetInstance()->Update();
	RenderTexture::GetInstance()->GetPostEffect()->Adjustment();

	// ImGui受付終了
	ImGuiManager::GetInstance()->End();
}
