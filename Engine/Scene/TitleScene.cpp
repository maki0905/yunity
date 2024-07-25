#include "TitleScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "RenderTexture.h"
#include "ObjectManager.h"
#include "ImGuiManager.h"

void TitleScene::Initialize()
{
	//camera_.reset(CameraManager::GetInstance()->GetCamera());
	camera_ = CameraManager::GetInstance()->GetCamera();
	worldTransform_.Initialize();

	world_ = std::make_unique<World>();
	world_->Initialize({ 0.0f, -9.8f, 0.0f });

	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ScoreBackground.png"), { 0.0f, 0.0f }));
	sprite_->SetSize({ 128.0f, 128.0f });
	sprite_->SetTextureRect({ 0.0f, 576.0f}, {64.0f, 64.0f});

	model_ = std::make_unique<Model>();
	model_.reset(ModelManager::GetInstance()->CreateModel(obj, "Cube"));
	//model_ = std::make_unique<Model>();
	//ModelManager::GetInstance()->CreateModel(obj, "terrain");
	model_->SetCamera(camera_/*camera_.get()*/);
	model_->SetLighting(false);

	/*obj_ = std::make_unique<Object3D>();
	obj_->Initialize(ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), Collider::kAABB);
	obj_->SetCamera(camera_);
	obj_->SetMass(2.0f);
	world_->Add(obj_.get());*/

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_/*camera_.get()*/, world_.get());
	camera_->SetTarget(nullptr);  

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
	if (player_->GetWorldTransform()->GetMatWorldTranslation().x >= 35.0f) {
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}
	else if (player_->GetWorldTransform()->GetMatWorldTranslation().x < -30.0f) {
		player_->GetWorldTransform()->translation_.x = -30.0f;
		Vector3 velocity = player_->GetVelocity();
		velocity.x *= -0.2f;
		player_->SetVelocity(velocity);
	}
	/*if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {
			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				SceneManager::GetInstance()->ChangeScene("SELECT");
				RenderTexture::GetInstance()->ClearPostEffect();
			}


		}
	}*/

	/*ImGui::Begin("Player");
	Vector3 pos = obj_->GetWorldTransform()->translation_;
	ImGui::DragFloat3("pos", &pos.x);
	obj_->SetPosition(pos);
	ImGui::End();

	obj_->AddForce(obj_->Spring({ 0.0f, 0.0f, 0.0f }, obj_->GetMatWorldTranslation(), 0.0f, 1.0f, 0.1f), 0);*/

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
	//obj_->Draw();
}

void TitleScene::DrawFront()
{
	sprite_->Draw();
}

