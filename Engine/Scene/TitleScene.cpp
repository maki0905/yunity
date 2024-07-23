#include "TitleScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "RenderTexture.h"
#include "ObjectManager.h"

void TitleScene::Initialize()
{
	//camera_.reset(CameraManager::GetInstance()->GetCamera());
	camera_ = CameraManager::GetInstance()->GetCamera();
	worldTransform_.Initialize();

	world_ = std::make_unique<World>();
	world_->Initialize({ 0.0f, -9.8f, 0.0f });

	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("TITLE.png"), { 320.0f, 260.0f }));

	model_ = std::make_unique<Model>();
	model_.reset(ModelManager::GetInstance()->CreateModel(obj, "SelectPoint"));
	//model_ = std::make_unique<Model>();
	//ModelManager::GetInstance()->CreateModel(obj, "terrain");
	model_->SetCamera(camera_/*camera_.get()*/);
	model_->SetLighting(false);

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_/*camera_.get()*/, world_.get());
	camera_->SetTarget(player_->GetWorldTransform());

	ObjectManager::GetInstance()->Load("title", camera_/*camera_.get()*/, world_.get());
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kHSVFilter, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kGrayscale, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kVignetting, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kOutline, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, true);
	/*RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kGrayscale)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kVignetting)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kOutline)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kRadialBlur)] = true;*/
}

void TitleScene::Update()
{
	prePad_ = pad_;

	player_->Update();
	/*if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {
			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				SceneManager::GetInstance()->ChangeScene("SELECT");
				RenderTexture::GetInstance()->ClearPostEffect();
			}


		}
	}*/

	world_->Solve();

	worldTransform_.UpdateMatrix();
}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	ObjectManager::GetInstance()->Draw("title");
	player_->Draw();
	//model_->Draw(worldTransform_);
}

void TitleScene::DrawFront()
{
	//sprite_->Draw();
}

