#include "SelectScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "ImGuiManager.h"
#include "CommonData.h"

void SelectScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	/*Vector3 pos = camera_->GetTranslate();
	pos.x = -15.0f;
	camera_->SetTranslate(pos);*/
	camera_->SetFixedAxis({ 0.0f, 1.0f, 1.0f });
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera_->SetTranslate({ 0.0f, 5.0f, -50.0f });
	camera_->SetTarget(nullptr);

	for (uint32_t index = 0; index < 3; index++) {
		models_[index] = std::make_unique<Model>();
		models_[index].reset(ModelManager::GetInstance()->CreateModel(obj, "TV"));
		models_[index]->SetCamera(camera_);
		models_[index]->SetEnableLighting(false);
		worldTransform_[index].Initialize();
		worldTransform_[index].translation_ = { 30.0f * index, 18.0f, 12.0f };
		worldTransform_[index].rotation_.x = -21.0f * DegToRad();
		worldTransform_[index].scale_ = { 0.0f, 0.0f, 0.0f };
		isActiveTV_[index] = false;
		grow_[index] = {false, 0.0f};
		shrink_[index] = {false, 0.0f};
	}
	textureTV_[0] = TextureManager::Load("Models/TV/TV1.png");
	textureTV_[1] = TextureManager::Load("Models/TV/TV2.png");
	textureTV_[2] = TextureManager::Load("Models/TV/TV3.png");
	
	preNum_ = CommonData::GetInstance()->stageNum_;

	world_ = std::make_unique<World>();
	world_->Initialize({ 0.0f, -15.0f, 0.0f });

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_/*camera_.get()*/, world_.get());
	player_->SetPosition({ -35.0f, -3.5f, 0.0f });

	if (CommonData::GetInstance()->stageNum_ != -1) {
		player_->SetPosition({ 30.0f * CommonData::GetInstance()->stageNum_, -3.5f, 0.0f});
		player_->SetSelect(true);
		worldTransform_[CommonData::GetInstance()->stageNum_].scale_ = { 1.0f, 1.0f, 1.0f };
		isActiveTV_[CommonData::GetInstance()->stageNum_] = true;
	}

	//camera_->SetTarget(player_->GetWorldTransform());
	ObjectManager::GetInstance()->Load("select", camera_/*camera_.get()*/, world_.get());

	/*sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("SELECT.png"), { 320.0f, 260.0f }));*/
	Input::GetInstance()->GetJoystickState(0, pad_);

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(camera_/*camera_.get()*/, { 5.0f, 5.0f, 5.0f });
	CommonData::GetInstance()->scene_ = Scene::kSelect;
}

void SelectScene::Update()
{
	prePad_ = pad_;

	if (player_->GetSelect()) {
		if (preNum_ != CommonData::GetInstance()->stageNum_) {
			grow_[CommonData::GetInstance()->stageNum_] = { true, 0.0f, worldTransform_[CommonData::GetInstance()->stageNum_].scale_};
		}

		for (uint32_t index = 0; index < 3; index++) {
			if (index != CommonData::GetInstance()->stageNum_) {
				grow_[index] = { false, 0.0f };
			}
		}

		isActiveTV_[CommonData::GetInstance()->stageNum_] = true;

		if (Input::GetInstance()->IsControllerConnected()) {
			if (Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
				}
			}
		}
	}
	else {
		for (uint32_t index = 0; index < 3; index++) {
			if (isActiveTV_[index] && !shrink_[index].flag) {
				shrink_[index] = { true, 0.0f, worldTransform_[index].scale_};
			}
		}

	}

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

	for (uint32_t index = 0; index < 3; index++) {
		if (grow_[index].flag) {
			grow_[index].t += 1.0f / 60.0f;
			if (grow_[index].t > 1.0f) {
				grow_[index].flag = false;
			}
			worldTransform_[index].scale_ = Lerp(/*grow_[index].scale*/{ 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f}, std::clamp(grow_[index].t, 0.0f, 1.0f));
		}
		if (shrink_[index].flag) {
			shrink_[index].t += 1.0f / 60.0f;
			if (shrink_[index].t > 1.0f) {
				shrink_[index].flag = false;
				isActiveTV_[index] = false;
			}
			worldTransform_[index].scale_ = Lerp(shrink_[index].scale, {0.0f, 0.0f, 0.0f}, std::clamp(shrink_[index].t, 0.0f, 1.0f));
		}
	}

	preNum_ = CommonData::GetInstance()->stageNum_;
	CommonData::GetInstance()->stageNum_ = -1;
	world_->Solve();

	for (uint32_t index = 0; index < 3; index++) {
		worldTransform_[index].UpdateMatrix();
	}

	if (Input::GetInstance()->TriggerKey(DIK_T)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

//#ifdef _DEBUG
//	ImGui::Begin("a");
//	ImGui::DragFloat3("pos", &worldTransform_.translation_.x);
//	Vector3 r = Multiply(RadToDeg(), worldTransform_.rotation_);
//	ImGui::DragFloat3("rotate", &r.x);
//	worldTransform_.rotation_ = Multiply(DegToRad(), r);
//	ImGui::DragFloat3("scale", &worldTransform_.scale_.x);
//	ImGui::End();
//
//#endif

}

void SelectScene::DrawBack()
{
}

void SelectScene::Draw3D()
{
	skydome_->Draw();
	ObjectManager::GetInstance()->Draw("select");
	for (uint32_t index = 0; index < 3; index++) {
		if (isActiveTV_[index]) {
			models_[index]->Draw(worldTransform_[index], textureTV_[index]);
		}
	}
	player_->Draw();
}

void SelectScene::DrawFront()
{
	//sprite_->Draw();
}
