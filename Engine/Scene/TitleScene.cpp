#include "TitleScene.h"

#include "Screen.h"
#include "TextureManager.h"


void TitleScene::Initialize()
{
	camera_.Initialize();

	title_ = std::make_unique<Model>();
	title_.reset(Model::Create("title"));

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(Model::Create("skydome"));

	pushA_ = std::make_unique<Sprite>();
	pushA_.reset(Sprite::Create(TextureManager::GetInstance()->Load("PushA.png"), { 0.0f, 0.0f }));


	titleTransform_.Initialize();
	titleTransform_.translation_ = { 0.5f, 10.0f, -20.0f };
	//titleTransform_.scale_ = { 10.0f, 1.0f, 1.0f };

	fishing_ = 0.0f;
	rotationSpeed_ = 0.2f;
	time_ = 0;
	display_ = false;
	flag_ = false;
}

void TitleScene::Update()
{
	if (!Screen::GetInstance()->GetActive() && !flag_) {
		flag_ = true;
	}

	if (flag_) {
		Input::GetInstance()->GetJoystickState(0, joyState_);


		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			Screen::GetInstance()->Playback(Screen::Type::Fade_Out_In, 2.0f, 2.0f);
		}
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && !(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y)) {
			rotationSpeed_ += 1.0f;
		}
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
			rotationSpeed_ += 1.0f;
		}
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) && !(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			rotationSpeed_ += 1.0f;
		}
		rotationSpeed_ = std::clamp(rotationSpeed_, 0.2f, 5.0f);
		if (Screen::GetInstance()->GetFlap()) {
			sceneNo_ = GAME_STAGE;
		}

	}

	const float step = 2.0f * std::numbers::pi_v<float> / 90.0f;

	fishing_ += step;
	fishing_ = std::fmodf(fishing_, float(2.0f * std::numbers::pi_v<float>));
	titleTransform_.translation_.y += std::sinf(fishing_) * (0.1f / 2.0f);


	if (rotationSpeed_ > 0.2f) {
		rotationSpeed_ -= 1.0f / 60.0f;
	}
	else {
		rotationSpeed_ = 0.2f;
	}
	titleTransform_.rotation_.y += rotationSpeed_ / 60.0f;

	titleTransform_.UpdateMatrix(RotationType::Euler);
	//time_++;
	if (time_++ % 60 == 0) {
		display_ ^= true;
		//time_ = 0;
	}

	camera_.UpdateMatrix();

	preJoyState_ = joyState_;

}

void TitleScene::DrawBack()
{
	
}

void TitleScene::Draw3D()
{
	skydome_->Draw(camera_);
	title_->Draw(titleTransform_, camera_);
}

void TitleScene::DrawFront()
{
	if (display_ && !Screen::GetInstance()->GetActive()) {
		pushA_->Draw();
	}
}

