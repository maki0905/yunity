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

	model_ = new Model();
	model_ = ModelManager::GetInstance()->CreateModel(obj, "terrain");
	//model_ = std::make_unique<Model>();
	//ModelManager::GetInstance()->CreateModel(obj, "terrain");
	model_->SetCamera(camera_/*camera_.get()*/);
	model_->SetLighting(false);
	RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kGrayscale, true);
	RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kVignetting, true);
	RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kOutline, true);
	RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, true);
	/*RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kGrayscale)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kVignetting)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kOutline)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kRadialBlur)] = true;*/
}

void TitleScene::Update()
{
	prePad_ = pad_;
	if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {
			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				SceneManager::GetInstance()->ChangeScene("SELECT");
				RenderTexture::GetInstance()->ClearPostEffect();
			}


		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}

	worldTransform_.UpdateMatrix();
}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	model_->Draw(worldTransform_);
}

void TitleScene::DrawFront()
{
	sprite_->Draw();
}

