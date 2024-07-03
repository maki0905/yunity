#include "SelectScene.h"

#include "SceneManager.h"
#include "ModelManager.h"

void SelectScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	worldTransform_.Initialize();

	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("SELECT.png"), { 320.0f, 260.0f }));
	Input::GetInstance()->GetJoystickState(0, pad_);
}

void SelectScene::Update()
{
	prePad_ = pad_;
	if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {
			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
			}


		}
	}

	worldTransform_.UpdateMatrix();
}

void SelectScene::DrawBack()
{
}

void SelectScene::Draw3D()
{
}

void SelectScene::DrawFront()
{
	sprite_->Draw();
}
