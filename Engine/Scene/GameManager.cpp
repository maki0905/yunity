#include "GameManager.h"

#include "Device.h"
#include "Input.h"

GameManager::GameManager()
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

	// 3Dオブジェクト
	model_ = new Model();
	model_->StaticInitialize();

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

	sphere_ = new SphereDrawer();
	sphere_->StaticInitialize();

#pragma endregion

	// 各シーンの配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[GAME_STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();

	// 初期シーンの設定
	currentSceneNo_ = GAME_STAGE;
	sceneArr_[currentSceneNo_]->Initialize();

}

GameManager::~GameManager()
{
	windowsAPI_->TerminateGameWindow();
}

void GameManager::Run()
{
	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		if (windowsAPI_->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager_->Begin();

		input_->Update();

		// シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		// シーン変更チェック
		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Initialize();
		}

		// 更新
		sceneArr_[currentSceneNo_]->Update();

		// ImGui受付終了
		imguiManager_->End();

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
	imguiManager_->Finalize();
}
