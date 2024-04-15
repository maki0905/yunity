#include "TitleScene.h"

#include "SceneManager.h"

void TitleScene::Initialize()
{
	camera_ = std::make_unique<Camera>();

	title_ = std::make_unique<Sprite>();
	title_.reset(Sprite::Create(TextureManager::GetInstance()->Load("title.png"), { 0.0f, 0.0f }));

	pushA_ = std::make_unique<Sprite>();
	pushA_.reset(Sprite::Create(TextureManager::GetInstance()->Load("push.png"), { 0.0f, 0.0f }));

	timer_ = 0;
	isDraw_ = true;
}

void TitleScene::Update()
{
	Input::GetInstance()->GetJoystickState(0, pad_);

	if (pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
	}

	if (timer_ % 60 == 0) {
		isDraw_ ^= true;
	}
	timer_++;

}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	
}

void TitleScene::DrawFront()
{
	title_->Draw();
	if (isDraw_) {
		pushA_->Draw();
	}
	
	
}

