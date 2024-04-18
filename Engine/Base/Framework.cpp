#include "Framework.h"

void Framework::Initialize()
{
	// WindowsAPI
	windowsAPI_ = WindowsAPI::GetInstance();
	windowsAPI_->CreateGameWindow();

	// DirectX基盤
	directXCore_ = DirectXCore::GetInstance();
	directXCore_->Initialize();

#pragma region 汎用機能初期化

#ifdef _DEBUG
	// ImGui
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize();
#endif // _DEBUG

	// Input
	input_ = Input::GetInstance();
	input_->Initialize();


	modelManager_ = ModelManager::GetInstance();
	modelManager_->Initialize();
	// 3Dオブジェクト
	/*model_ = new Model();
	model_->StaticInitialize();*/

	sprite_ = new Sprite();
	sprite_->StaticInitialize();

	primitive_ = new PrimitiveDrawer();
	primitive_->StaticInitialize();

	particle_ = new ParticleDrawer();
	particle_->StaticInitialize();

	// テクスチャマネージャー
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize(Device::GetInstance()->GetDevice());
	TextureManager::Load("white1x1.png");

	/*sphere_ = new SphereDrawer();
	sphere_->StaticInitialize();*/

	audio_ = Audio::GetInstance();
	audio_->Initialize();

	sceneManager_ = SceneManager::GetInstance();
	sceneManager_->Initialize();
	sceneManager_->ChangeScene("CLEAR");

	csv_ = CSV::GetInstance();
	csv_->Initialize();



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
	imguiManager_->Finalize();
	windowsAPI_->TerminateGameWindow();
}

void Framework::Update()
{
	// ImGui受付開始
	imguiManager_->Begin();

	input_->Update();

	// シーンのチェック
	/*prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();*/

	// シーン変更チェック
	/*if (prevSceneNo_ != currentSceneNo_) {
		sceneArr_[currentSceneNo_]->Initialize();
	}*/

	// 更新
	//sceneArr_[currentSceneNo_]->Update();

	sceneManager_->Update();

	// ImGui受付終了
	imguiManager_->End();
}
