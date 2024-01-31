#include "ClearScene.h"

#include "Screen.h"
#include "TextureManager.h"

void ClearScene::Initialize()
{
	camera_.Initialize();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(Model::Create("skydome"));

	clear_ = std::make_unique<Sprite>();
	clear_.reset(Sprite::Create(TextureManager::GetInstance()->Load("Clear.png"), { 0.0f, 0.0f }));

	pushA_ = std::make_unique<Sprite>();
	pushA_.reset(Sprite::Create(TextureManager::GetInstance()->Load("PushA.png"), { 0.0f, 0.0f }));

	fishing_ = 0.0f;
	rotationSpeed_ = 0.2f;
	time_ = 0;
	display_ = false;
	flag_ = false;
}

void ClearScene::Update()
{
	Input::GetInstance()->GetJoystickState(0, joyState_);

	if (Screen::GetInstance()->GetOver() != flag_ && !flag_) {
		flag_ = true;
	}

	if (flag_) {
		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			Screen::GetInstance()->Playback(Screen::Type::Fade_Out_In, 2.0f, 2.0f);
		}

		if (Screen::GetInstance()->GetFlap()) {
			sceneNo_ = TITLE;
		}
	}



	if (time_++ % 60 == 0) {
		display_ ^= true;
		//time_ = 0;
	}

	camera_.UpdateMatrix();

	preJoyState_ = joyState_;
}

void ClearScene::DrawBack()
{
}

void ClearScene::Draw3D()
{
	skydome_->Draw(camera_);
}

void ClearScene::DrawFront()
{
	clear_->Draw();
	if (display_ && !Screen::GetInstance()->GetActive()) {
		pushA_->Draw();
	}
}
