#include "TitleScene.h"

#include "TextureManager.h"

void TitleScene::Initialize()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::Load("title.png"), { 0.0f, 0.0f }));
	sprite_->SetSize(Vector2(1280.0f, 720.0f));
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo_ = GAME_STAGE;
	}

}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
}

void TitleScene::DrawFront()
{
	sprite_->Draw();
}

