#include "TitleScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "RenderTexture.h"


void TitleScene::Initialize()
{
	//camera_.reset(CameraManager::GetInstance()->GetCamera());
	camera_ = CameraManager::GetInstance()->GetCamera();
	worldTransform_.Initialize();

	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("TITLE.png"), { 320.0f, 260.0f }));

	//model_ = std::make_unique<Model>();
	//model_.reset(ModelManager::GetInstance()->CreateModel(obj, "terrain"));
	//model_->SetCamera(camera_/*camera_.get()*/);
	//model_->SetLighting(false);
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kGrayscale)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kVignetting)] = true;
}

void TitleScene::Update()
{
	prePad_ = pad_;
	if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {
			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				SceneManager::GetInstance()->ChangeScene("SELECT");
			}


		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
	}

	worldTransform_.UpdateMatrix();
}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	//model_->Draw(worldTransform_);
}

void TitleScene::DrawFront()
{
	sprite_->Draw();
}

