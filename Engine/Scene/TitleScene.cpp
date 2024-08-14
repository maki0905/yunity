#include "TitleScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "RenderTexture.h"
#include "ObjectManager.h"
#include "ImGuiManager.h"
#include "CommonData.h"

void TitleScene::Initialize()
{
	//camera_.reset(CameraManager::GetInstance()->GetCamera());
	camera_ = CameraManager::GetInstance()->GetCamera();
	world_ = std::make_unique<World>();
	world_->Initialize({ 0.0f, -15.0, 0.0f });

	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ScoreBackground.png"), { 0.0f, 0.0f }));
	sprite_->SetSize({ 128.0f, 128.0f });
	sprite_->SetTextureRect({ 0.0f, 576.0f}, {64.0f, 64.0f});

	model_ = std::make_unique<Model>();
	model_.reset(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Signboard"));
	//model_ = std::make_unique<Model>();
	//ModelManager::GetInstance()->CreateModel(obj, "terrain");
	Model::DirectionalLight l = { .color = {1.0f, 1.0f, 1.0f, 1.0f}, .direction = {-1.0f, -1.0f, 0.0f}, .intensity= 1.0f };
	model_->SetCamera(camera_/*camera_.get()*/);
	model_->SetEnableLighting(true);
	model_->SetDirectionalLight(l);
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 30.0f, -7.0f, 6.0f };

	/*obj_ = std::make_unique<Object3D>();
	obj_->Initialize(ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), Collider::kAABB);
	obj_->SetCamera(camera_);
	obj_->SetMass(0.0f);
	obj_->SetCollisionAttribute(kCollisionAttributeSpike);
	world_->Add(obj_.get());*/

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_/*camera_.get()*/, world_.get());
	player_->SetDirectionalLight(l);
	camera_->SetTarget(nullptr);  

	ObjectManager::GetInstance()->Load("stage2", camera_/*camera_.get()*/, world_.get());
	ObjectManager::GetInstance()->SetDirectionalLight("title", l);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kHSVFilter, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kGrayscale, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kVignetting, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kOutline, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, true);
	/*RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kGrayscale)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kVignetting)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kOutline)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kRadialBlur)] = true;*/

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(camera_/*camera_.get()*/, { 5.0f, 5.0f, 5.0f });

	CommonData::GetInstance()->stageNum_ = -1;
	CommonData::GetInstance()->scene_ = Scene::kTitle;
}

void TitleScene::Update()
{
	prePad_ = pad_;

	/*Vector3 rotate{ 0.4f, 1.43f, -0.8f };
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZ = Multiply(rotateX, Multiply(rotateY, rotateZ));*/

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

	i += k;
	if (i > 10.0f || i < -10.0f) {
		k *= -1.0f;
	}
	worldTransform_.rotation_.z = i * DegToRad();
	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("a");
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x);
	if (ImGui::Button("Off")) {
		model_->SetEnableLighting(false);
		player_->SetEnableLighting(false);
		ObjectManager::GetInstance()->SetEnableLighting("title",false);
	}
	if (ImGui::Button("On")) {
		model_->SetEnableLighting(true);
		player_->SetEnableLighting(true);
		ObjectManager::GetInstance()->SetEnableLighting("title", true);
	}
	ImGui::End();

#endif

}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	skydome_->Draw();
	ObjectManager::GetInstance()->Draw("stage2");
	player_->Draw();
	model_->Draw(worldTransform_);
	//obj_->Draw();
}

void TitleScene::DrawFront()
{
}

