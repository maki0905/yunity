#include "StageScene.h"

#include "TextureManager.h"
#include "Input.h"

void StageScene::Initialize()
{
	textureHandle_ = TextureManager::Load("uvChecker.png");
	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(textureHandle_, { 0.0f, 0.0f }));
	position_ = { 0.0f, 0.0f };

	camera_.Initialize();
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix(RotationType::Euler);
	worldTrasnform1_.Initialize();
	//worldTrasnform1_.translation_.y = 0.0f;
	worldTrasnform1_.UpdateMatrix(RotationType::Euler);

	skydome_.reset(Model::Create("skydome"));

	sphere_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
	player_ = new Player();
	player_->Initialize();

	/*particle_ = new ParticleDrawer();
	particle_ = ParticleDrawer::Create("uvChecker.png");*/
	particle_ = std::make_unique<ParticleDrawer>();
	particle_.reset(ParticleDrawer::Create("uvChecker.png"));
	
	block_ = new Block();
	block_->Initialize();
	//player_ = std::make_unique<Player>();
	/*skydome1_ = std::make_unique<Skydome>();
	skydome1_->Initialize(Model::Create("skydome"));*/
	//player_->Initialize();

	for (uint32_t index = 0; index < 10; index++) {
		worldTransformParticle_[index].Initialize();
		worldTransformParticle_[index].scale_ = { 5.0f, 5.0f, 5.0f };
		worldTransformParticle_[index].translation_ = { rng.NextFloatRange(-1.0f, 1.0f), rng.NextFloatRange(-1.0f, 1.0f), rng.NextFloatRange(-1.0f, 1.0f) };
		worldTransformParticle_[index].UpdateMatrix(RotationType::Euler);
		velocity_[index] = { rng.NextFloatRange(-0.1f, 0.1f), rng.NextFloatRange(-0.1f, 0.1f), 0.0f };
		
	}

	collisionManager_ = std::make_unique<CollisionManager>();

}

void StageScene::Update()
{
	// 衝突マネージャーのリストをクリア
	collisionManager_->ClearCollider();
	// コライダーを衝突マネージャーのリストに登録
	collisionManager_->SetCollider(player_);
	collisionManager_->SetCollider(block_);
	// 衝突マネージャーの当たり判定処理を呼び出す
	collisionManager_->CheckAllCollision();


	//player_->Update();
	player_->Update();

	if (index_particle > 10) {
		index_particle = 0;
	}

	for (uint32_t index = 0; index < 10; index++) {
		worldTransformParticle_[index].translation_ = Add(worldTransformParticle_[index].translation_, velocity_[index]);
		worldTransformParticle_[index].UpdateMatrix(RotationType::Euler);
	}

	worldTrasnform1_.UpdateMatrix(RotationType::Euler);
	camera_.UpdateMatrix();
	
}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{
	/*skydome_->Draw(worldTransform_, camera_);
	sphere_->Draw(worldTrasnform1_, camera_);*/
	//skydome_->Draw(worldTransform_, camera_);
	//particle_->Draw(worldTrasnform1_, camera_);
	particle_->Draw(worldTransformParticle_, camera_);
	player_->Draw(camera_);
	/*block_->Draw(camera_);*/
}

void StageScene::DrawFront()
{
	//sprite_->Draw();
}
