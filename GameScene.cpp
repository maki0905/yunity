#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene(){}

GameScene::~GameScene(){}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	viewProjection_.Initialize();

	// デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();

	collisionManager_ = std::make_unique<CollisionManager>();

	// モデル
	skydomeModel_.reset(Model::Create("skydome"));
	floorModel_.reset(Model::Create("floor"));
	std::vector<Model*> floorModels = {
		floorModel_.get(),
	};
	movingFloorModel_.reset(Model::Create("Yfloor"));
	std::vector<Model*> movingFloorModels = {
		movingFloorModel_.get(),
	};

	playerModel_Body_.reset(Model::Create("float_Body"));
	playerModel_Head_.reset(Model::Create("float_Head"));
	playerModel_Larm_.reset(Model::Create("float_L_arm"));
	playerModel_Rarm_.reset(Model::Create("float_R_arm"));
	std::vector<Model*> playerModles = {
		playerModel_Body_.get(), playerModel_Head_.get(), playerModel_Larm_.get(), playerModel_Rarm_.get()
	};

	enemyModel_Body_.reset(Model::Create("needle_Body"));
	enemyModel_Larm_.reset(Model::Create("needle_L_arm"));
	enemyModel_Rarm_.reset(Model::Create("needle_R_arm"));
	std::vector<Model*> enemyModles = {
		enemyModel_Body_.get(), enemyModel_Larm_.get(), enemyModel_Rarm_.get()
	};

	startBoxModel_.reset(Model::Create("startBox"));
	
	endBoxModel_.reset(Model::Create("endBox"));

	/// <summary>
	/// オブジェクト
	/// </summary>
	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());
	// 動かない床
	for (int i = 0; i < 3; i++) {
		Floor* floor_ = new Floor();
		floor_->Initialize(floorModels);
		floors_.emplace_back(floor_);
	}
	floors_[0]->SetPosition({ 0.0f, 0.0f, 0.0f });
	floors_[1]->SetPosition({ 0.0f, 0.0f, 100.0f });
	floors_[2]->SetPosition({ 100.0f, 0.0f, 100.0f });


	// 動く床
	for (int i = 0; i < 2; i++) {
		MovingFloor* movingFloor = new MovingFloor();
		movingFloor->Initialize(movingFloorModels);
		movingFloors_.emplace_back(movingFloor);
	}
	movingFloors_[0]->SetPosition({ 0.0f, 0.0f, 50.0f });
	movingFloors_[0]->SetVelocity({ 0.0f, 0.0f, 0.1f });
	movingFloors_[1]->SetPosition({ 50.0f, 0.0f, 100.0f });
	movingFloors_[1]->SetVelocity({ 0.1f, 0.0f, 0.0f });

	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModles);
	// 敵
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModles);

	startBox_ = std::make_unique<StartBox>();
	startBox_->Initialize(startBoxModel_.get());
	endBox_ = std::make_unique<EndBox>();
	endBox_->Initialize(endBoxModel_.get());

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize(viewProjection_);
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(followCamera_->GetViewProjection());

}

void GameScene::Update()
{
	if (player_->GetIsAlive()) {
		// 衝突マネージャーのリストをクリア
		collisionManager_->ClearCollider();
		// コライダーを衝突マネージャーのリストに登録
		collisionManager_->SetCollider(player_.get());
		collisionManager_->SetCollider(enemy_.get());
		for (auto& floor_ : floors_) {
			collisionManager_->SetCollider(floor_);
		}
		for (auto& movingFloor : movingFloors_) {
			collisionManager_->SetCollider(movingFloor);
		}
		collisionManager_->SetCollider(endBox_.get());
		// 衝突マネージャーの当たり判定処理を呼び出す
		collisionManager_->CheckAllCollision();

		for (auto& movingFloor : movingFloors_) {
			movingFloor->Update();
		}
		player_->Update();
		enemy_->Update();

		followCamera_->Update();

		if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
			isDebugCameraFlag_ ^= true;
		}

		if (isDebugCameraFlag_) {
			debugCamera_->Update(&viewProjection_);
			viewProjection_.UpdateMatrix();
		}
		else {
			viewProjection_.matView = followCamera_->GetViewProjection()->matView;
			viewProjection_.matProjection = followCamera_->GetViewProjection()->matProjection;
		}
	}
	else {
		Reset();
	}
	

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
	for (auto& movingFloor : movingFloors_) {
		movingFloor->Draw(viewProjection_);
	}
	for (auto& floor : floors_) {
		floor->Draw(viewProjection_);
	}
	startBox_->Draw(viewProjection_);
	endBox_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
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

void GameScene::Reset()
{
	player_->Reset();
}
