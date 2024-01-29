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
	debugCamera_ = std::make_unique<DebugCamera>();

	

}

void StageScene::Update()
{
	

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
	}
#endif

	if (isDebug_) {
		debugCamera_->Update(&camera_);
		camera_.UpdateMatrix();
	}
	else {
		camera_.UpdateMatrix();
	}

	
}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{
	
}

void StageScene::DrawFront()
{
	//sprite_->Draw();
}
