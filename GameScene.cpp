#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene(){}

GameScene::~GameScene(){
}

void GameScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	viewProjection_.Initialize();

	// デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();

	collisionManager_ = std::make_unique<CollisionManager>();

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initalize();

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
	playerModel_Hammer_.reset(Model::Create("hammer"));
	std::vector<Model*> playerModles = {
		playerModel_Body_.get(), playerModel_Head_.get(), playerModel_Larm_.get(), playerModel_Rarm_.get(), playerModel_Hammer_.get()
	};
	std::vector<Model*>weaponModels = {
		playerModel_Hammer_.get()
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
	floors_[1]->SetScale(Vector3{ 2.0f, 0.5f, 2.0f });
	floors_[1]->SetPosition({ -15.0f, 0.0f, 115.0f });
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
	//weapon_ = std::make_unique<PlayerWeapon>();
	//weapon_->Initialize(weaponModels);
	// 敵

	enemies_.clear();
	enemyPos_[0] = Vector3{ 0.0f, 2.0f, 100.0f };
	enemyPos_[1] = Vector3{ 0.0f, 2.0f, 130.0f };
	enemyPos_[2] = Vector3{ -30.0f, 2.0f, 100.0f };
	enemyPos_[3] = Vector3{ -30.0f, 2.0f, 130.0f };
	enemyPos_[4] = Vector3{ -15.0f, 2.0f, 115.0f };

	for (int i = 0; i < 5; i++) {
		Enemy* enemy = new Enemy();
		enemy->Initialize(enemyModles);
		enemy->SetTranslation(enemyPos_[i]);
		enemies_.emplace_back(enemy);
	}


	/*enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModles);*/

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

	particle_.reset(Model::Create("particle"));
	explosion_.clear();


	/*pirticle_ = std::make_unique<Pirticle>();
	pirticle_.reset(Pirticle::Create("Resurces/uvChacker.png"));*/

	//explosion_ = std::make_unique<Explosion>();
	//explosion_->Initialize(/*pirticle_.get()*/);
	
}

void GameScene::Update()
{
	if (player_->GetIsAlive()) {
		// 衝突マネージャーのリストをクリア
		collisionManager_->ClearCollider();
		// コライダーを衝突マネージャーのリストに登録
		collisionManager_->SetCollider(player_.get());
		/*if (weapon_->GetIsActive()) {
			collisionManager_->SetCollider(weapon_.get());
		}*/
		/*if (enemy_->GetIsActive()) {
			collisionManager_->SetCollider(enemy_.get());
		}*/
		for (auto& enemy : enemies_) {
			if (!enemy->GetIsDestroy()) {
				collisionManager_->SetCollider(enemy.get());
			}
		}
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
		//weapon_->Update();
		//enemy_->Update();

		for (auto& enemy : enemies_) {
			enemy->Update();
			if (enemy->GetSwich()) {
				for (int i = 0; i < 6; i++) {

					Explosion* explosion = new Explosion();
					float angle = 60.0f * float(i);
					Vector3 velocity = { std::cosf(angle), 0.5f, std::sinf(angle) };

					explosion->Initialize(particle_.get(), enemy->GetWorldPosition(), velocity);
					explosion_.push_back(explosion);
				}
			}
		}

		if (lockOn_->ExistTarget()) {
			followCamera_->SetLockOn(lockOn_.get());
			player_->SetLockOn(lockOn_.get());
		}
		followCamera_->Update();

		lockOn_->Update(enemies_, viewProjection_);

		for (auto& explosion : explosion_) {
			explosion->Update();


			
		}

		

		explosion_.remove_if([](Explosion* explosion) {
			if (explosion->GetIsDead()) {
				delete explosion;
				return true;
			}
			return false;
		});

		

#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
			isDebugCameraFlag_ ^= true;
		}
#endif

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
	//viewProjection_.UpdateMatrix();

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region パーティクル
	/*Pirticle::PreDraw(commandList);
	explosion_->Draw(viewProjection_);
	Pirticle::PostDraw();
	dxCommon_->ClearDepthBuffer();*/
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
	endBox_->Draw(viewProjection_);
	startBox_->Draw(viewProjection_);
	//enemy_->Draw(viewProjection_);

	for (auto& enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	player_->Draw(viewProjection_);

	for (auto& explosion : explosion_) {
		explosion->Draw(viewProjection_);
	}
	//weapon_->Draw(viewProjection_);
	/// </summary>
	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	lockOn_->Draw();
	/// </summary>

	// デバッグテキストの描画
	//debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::Reset()
{
	player_->Reset();
	for (auto& enemy : enemies_) {
		enemy->Reset();
	}
	//enemy_->Reset();
}
