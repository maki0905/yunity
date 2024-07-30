#include "SelectScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "ObjectManager.h"

void SelectScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	/*Vector3 pos = camera_->GetTranslate();
	pos.x = -15.0f;
	camera_->SetTranslate(pos);*/
	camera_->SetFixedAxis({ 0.0f, 1.0f, 1.0f });

	worldTransform_.Initialize();

	world_ = std::make_unique<World>();
	world_->Initialize({ 0.0f, -9.8f, 0.0f });

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_/*camera_.get()*/, world_.get());
	player_->SetPosition({ -35.0f, -3.5f, 0.0f });
	//camera_->SetTarget(player_->GetWorldTransform());
	ObjectManager::GetInstance()->Load("select1", camera_/*camera_.get()*/, world_.get());

	/*sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("SELECT.png"), { 320.0f, 260.0f }));*/
	Input::GetInstance()->GetJoystickState(0, pad_);
}

void SelectScene::Update()
{
	prePad_ = pad_;
	player_->Update();
	if (player_->GetWorldTransform()->GetMatWorldTranslation().x < -35.0f) {
		player_->GetWorldTransform()->translation_.x = -35.0f;
		Vector3 velocity = player_->GetVelocity();
		velocity.x *= -0.2f;
		player_->SetVelocity(velocity);
	}

	if (!camera_->GetTarget()) {
		if (player_->GetWorldTransform()->translation_.x > 0.0f) {
			camera_->SetTarget(player_->GetWorldTransform());
		}
	}
	else {
		if (player_->GetWorldTransform()->translation_.x < 0.0f) {
			camera_->SetTarget(nullptr);
		}
	}

	/*if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {
			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
			}


		}
	}*/

	world_->Solve();

	if (player_->GetSelect()) {
		if (Input::GetInstance()->IsControllerConnected()) {
			if (Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
				}
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
	ObjectManager::GetInstance()->Draw("select1");
	player_->Draw();
}

void SelectScene::DrawFront()
{
	//sprite_->Draw();
}
