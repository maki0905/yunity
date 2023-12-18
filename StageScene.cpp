#include "StageScene.h"

#include "TextureManager.h"
#include "Input.h"

void StageScene::Initialize()
{
	textureHandle_ = TextureManager::Load("white1x1.png");
	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(textureHandle_, { 0.0f, 0.0f }));
	position_ = { 0.0f, 0.0f };

	camera_.Initialize();
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix(RotationType::Euler);

	skydome_.reset(Model::Create("skydome"));

}

void StageScene::Update()
{
	worldTransform_.UpdateMatrix(RotationType::Euler);
	camera_.UpdateMatrix();
	if (Input::GetInstance()->PushKey(DIK_A)) {
		position_.x -= 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		position_.x += 1.0f;
	}
}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{
	skydome_->Draw(worldTransform_, camera_);
}

void StageScene::DrawFront()
{
	sprite_->Draw();
}
