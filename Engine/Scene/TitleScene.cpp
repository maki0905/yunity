#include "TitleScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "RenderTexture.h"
#include "ObjectManager.h"
#include "ImGuiManager.h"
#include "CommonData.h"
#include "Tradition.h"
#include "EngineTimeStep.h"

void TitleScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	world_ = std::make_unique<yunity::World>();
	world_->Initialize(gravity_);

	bottonSprite_ = std::make_unique<yunity::Sprite>();
	bottonSprite_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("ABotton.dds"), spritePos_));
	bottonPushSprite_ = std::make_unique<yunity::Sprite>();
	bottonPushSprite_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("ABottonPush.png"), spritePos_));
	time_ = 0;

	model_ = std::make_unique<yunity::Model>();
	model_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Signboard"));
	//model_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Brick"));
	DirectionLight directionLight;
	yunity::Model::DirectionalLight l = { .color = directionLight.color, .direction = directionLight.direction, .intensity = directionLight.intensity };
	model_->SetCamera(camera_);
	model_->SetEnableLighting(true);
	model_->SetDirectionalLight(l);
	worldTransform_.Initialize();
	worldTransform_.translation_ = signboardconstant_.pos;

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_, world_.get());
	player_->SetDirectionalLight(l);
	player_->SetMass(0.0f);
	player_->SetInGame(true);
	camera_->SetTarget(nullptr);

	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize();
	objectManager_->Load("title1", camera_, world_.get());
	objectManager_->SetDirectionalLight(l);


	skydome_ = std::make_unique<yunity::Skydome>();
	skydome_->Initialize(camera_, skydomeScale_);

	CommonData::GetInstance()->stageNum_ = -1;
	CommonData::GetInstance()->scene_ = Scene::kTitle;

	isStart_ = false;

	TVConstant tvConstant;
	for (uint32_t index = 0; index < tvCount_; index++) {
		models_[index] = std::make_unique<yunity::Model>();
		models_[index].reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "TV"));
		models_[index]->SetCamera(camera_);
		models_[index]->SetEnableLighting(false);
		TVworldTransform_[index].Initialize();
		TVworldTransform_[index].translation_ = { tvConstant.translation.x + tvConstant.addition * index, tvConstant.translation.y, tvConstant.translation.z };
		TVworldTransform_[index].rotation_.x = tvConstant.rotationX;
		TVworldTransform_[index].scale_ = { 0.0f, 0.0f, 0.0f };
		isActiveTV_[index] = false;
		grow_[index] = { false, 0.0f };
		shrink_[index] = { false, 0.0f };
	}
	textureTV_[0] = yunity::TextureManager::Load("Models/TV/TV1.png");
	textureTV_[1] = yunity::TextureManager::Load("Models/TV/TV2.png");
	textureTV_[2] = yunity::TextureManager::Load("Models/TV/TV3.png");

	preNum_ = CommonData::GetInstance()->stageNum_;
	camera_->SetTranslate(cameraPos_);
	CommonData::GetInstance()->scene_ = Scene::kTitle;
	isMoveCamera[0] = false;
	isMoveCamera[1] = false;
	moveCameraTimer_ = 0.0f;

	/*coin_ = std::make_unique<Coin>();
	coin_->Initialize();*/

}

void TitleScene::Update()
{
	prePad_ = pad_;

	if (player_->GetSelect()) {
		if (preNum_ != CommonData::GetInstance()->stageNum_) {
			grow_[CommonData::GetInstance()->stageNum_] = { true, 0.0f, TVworldTransform_[CommonData::GetInstance()->stageNum_].scale_ };
		}

		for (int index = 0; index < tvCount_; index++) {
			if (index != CommonData::GetInstance()->stageNum_) {
				grow_[index] = { false, 0.0f };
			}
		}

		isActiveTV_[CommonData::GetInstance()->stageNum_] = true;

		if (yunity::Input::GetInstance()->IsControllerConnected()) {
			if (yunity::Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					if (!Tradition::GetInstance()->GetIsActive()) {
						isMoveCamera[0] = true;
						oldCameraPos_ = camera_->GetTranslate();
						moveCameraTimer_ = 0.0f;
						Tradition::GetInstance()->Initialize();
						Tradition::GetInstance()->Start();
						camera_->SetTarget(nullptr);
						player_->SetVelocity({ 0.0f, 0.0f, 0.0f });
					}

				}
			}
		}
	}
	else {
		for (uint32_t index = 0; index < tvCount_; index++) {
			if (isActiveTV_[index] && !shrink_[index].flag) {
				shrink_[index] = { true, 0.0f, TVworldTransform_[index].scale_ };
			}
		}

	}

	if (!Tradition::GetInstance()->GetIsActive()) {
		yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, false);
		if (isStart_) {
			player_->SetMass(playerMass_);
			player_->Update();
		}
		if (player_->GetWorldTransform()->GetMatWorldTranslation().x >= targetPoint_ && camera_->GetTarget() == nullptr) {
			camera_->SetTarget(player_->GetWorldTransform());
			CommonData::GetInstance()->scene_ = Scene::kSelect;

		}
		else if (player_->GetWorldTransform()->GetMatWorldTranslation().x < targetPoint_ && camera_->GetTarget() != nullptr) {
			camera_->SetTarget(nullptr);
			camera_->SetTranslate(cameraPos_);
			CommonData::GetInstance()->scene_ = Scene::kTitle;

		}

		if (yunity::Input::GetInstance()->IsControllerConnected()) {
			if (yunity::Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					isStart_ = true;
				}
			}
		}
	}
	else {
		if (isMoveCamera[0]) {
			moveCameraTimer_ += yunity::fixedTimeStep_;
			moveCameraTimer_ = std::clamp(moveCameraTimer_, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(oldCameraPos_, { oldCameraPos_.x, endCamerPos.y, endCamerPos.z }, moveCameraTimer_));
			if (moveCameraTimer_ >= 1.0f) {
				moveCameraTimer_ = 0.0f;
				isMoveCamera[0] = false;
				oldCameraPos_ = camera_->GetTranslate();
				yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, true);
			}
		}
		else {
			moveCameraTimer_ += yunity::fixedTimeStep_;
			moveCameraTimer_ = std::clamp(moveCameraTimer_, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(oldCameraPos_, { oldCameraPos_.x, oldCameraPos_.y, fixedEndCameraPosZ }, moveCameraTimer_));
			Tradition::GetInstance()->Update();
		}
		if (!Tradition::GetInstance()->GetIn()) {
			yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, false);
			yunity::SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
		}
	}

	if (yunity::Input::GetInstance()->TriggerKey(DIK_0)) {
		yunity::SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
	}

	for (uint32_t index = 0; index < tvCount_; index++) {
		if (grow_[index].flag) {
			grow_[index].t += yunity::fixedTimeStep_;
			if (grow_[index].t > 1.0f) {
				grow_[index].flag = false;
			}
			TVworldTransform_[index].scale_ = Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, std::clamp(grow_[index].t, 0.0f, 1.0f));
		}
		if (shrink_[index].flag) {
			shrink_[index].t += yunity::fixedTimeStep_;
			if (shrink_[index].t > 1.0f) {
				shrink_[index].flag = false;
				isActiveTV_[index] = false;
			}
			TVworldTransform_[index].scale_ = Lerp(shrink_[index].scale, { 0.0f, 0.0f, 0.0f }, std::clamp(shrink_[index].t, 0.0f, 1.0f));
		}
	}

	preNum_ = CommonData::GetInstance()->stageNum_;
	CommonData::GetInstance()->stageNum_ = -1;

	for (uint32_t index = 0; index < tvCount_; index++) {
		TVworldTransform_[index].UpdateMatrix();
	}

	world_->Solve();

	wavelength_ += amplitude_;
	if (wavelength_ > signboardconstant_.limit || wavelength_ < -signboardconstant_.limit) {
		amplitude_ *= -1.0f;
	}
	worldTransform_.rotation_.z = wavelength_ * DegToRad();
	worldTransform_.UpdateMatrix();

	time_++;
	if (time_ % limitTime_ == 0) {
		time_ = 0;
		isDraw_ ^= true;

	}

#ifdef _DEBUG

	ImGui::Begin("a");

	ImGui::DragFloat3("pos", &worldTransform_.translation_.x);
	if (ImGui::Button("Off")) {
		model_->SetEnableLighting(false);
		player_->SetEnableLighting(false);
	}
	if (ImGui::Button("On")) {
		model_->SetEnableLighting(true);
		player_->SetEnableLighting(true);
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
	objectManager_->Draw();

	player_->Draw();
	model_->Draw(worldTransform_);

	for (uint32_t index = 0; index < tvCount_; index++) {
		if (isActiveTV_[index]) {
			models_[index]->Draw(TVworldTransform_[index], textureTV_[index]);
		}
	}
}

void TitleScene::DrawFront()
{
	if (!isStart_ || preNum_ > -1) {
		if (isDraw_) {
			bottonSprite_->Draw();
		}
		else {
			bottonPushSprite_->Draw();
		}
	}
	Tradition::GetInstance()->Draw();
}

