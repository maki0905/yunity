#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene(){}

GameScene::~GameScene(){}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	viewProjection_.Initialize();

	// モデル
	skydomeModel_.reset(Model::Create("skydome"));
	TextureManager::Load("Models/float_Body/tex.png");

	playerModel_Body_.reset(Model::Create("float_Body"));
	playerModel_Head_.reset(Model::Create("float_Head"));
	playerModel_Larm_.reset(Model::Create("float_L_arm"));
	playerModel_Rarm_.reset(Model::Create("float_R_arm"));
	std::vector<Model*> playerModles = {
		playerModel_Body_.get(), playerModel_Head_.get(), playerModel_Larm_.get(), playerModel_Rarm_.get()
	};

	// オブジェクト
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModles);
}

void GameScene::Update()
{

	//player_->Update();
	viewProjection_.UpdateMatrix();

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	//// 背景スプライト描画前処理
	//Sprite::PreDraw(commandList);

	///// <summary>
	///// ここに背景スプライトの描画処理を追加できる
	///// </summary>

	//// スプライト描画後処理
	//Sprite::PostDraw();
	//// 深度バッファクリア
	//dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	skydome_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	//// 前景スプライト描画前処理
	//Sprite::PreDraw(commandList);

	///// <summary>
	///// ここに前景スプライトの描画処理を追加できる
	///// </summary>

	//// デバッグテキストの描画
	//debugText_->DrawAll(commandList);
	////
	//// スプライト描画後処理
	//Sprite::PostDraw();

#pragma endregion

}
