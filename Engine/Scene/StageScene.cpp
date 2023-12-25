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

	sphere_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kSphere));

}

void StageScene::Update()
{
	worldTransform_.UpdateMatrix(RotationType::Euler);
	camera_.UpdateMatrix();
	
}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{
	skydome_->Draw(worldTransform_, camera_);
	sphere_->Draw(worldTrasnform1_, camera_);
}

void StageScene::DrawFront()
{
	//sprite_->Draw();
}
