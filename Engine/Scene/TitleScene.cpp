#include "TitleScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "RenderTexture.h"
#include "ObjectManager.h"
#include "ImGuiManager.h"
#include "CommonData.h"
#include "Tradition.h"
#include "EngineTimeStep.h"
#include "GlobalVariables.h"
#include "EngineTimeStep.h"

void TitleScene::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "TitleScene";
	cameraPos_ = globalVariables->GetVector3Value(groupName, "CameraPos");
	gravity_ = globalVariables->GetVector3Value(groupName, "Gravity");
	skydomeScale_ = globalVariables->GetVector3Value(groupName, "SkydomeScale");
	spritePos_ = globalVariables->GetVector2Value(groupName, "SpritePos");
	limitTime_ = globalVariables->GetFloatValue(groupName, "LimitTime");
	targetPoint_ = globalVariables->GetFloatValue(groupName, "TargetPoint");
	playerMass_ = globalVariables->GetFloatValue(groupName, "PlayerMass");
	DirectionLight directionLight;
	directionLight.color = globalVariables->GetVector4Value(groupName, "DirectionLightColor");
	directionLight.direction = globalVariables->GetVector3Value(groupName, "DirectionLightDirection");
	directionLight.eyePosition = globalVariables->GetVector3Value(groupName, "DirectionLightEyePosition");
	directionLight.targetPosition = globalVariables->GetVector3Value(groupName, "DirectionLightTargetPosition");
	directionLight.upDirection = globalVariables->GetVector3Value(groupName, "DirectionLightUpDirection");
	directionLight.intensity = globalVariables->GetFloatValue(groupName, "DirectionLightIntensity");
	directionLight.viewWidth = globalVariables->GetFloatValue(groupName, "DirectionLightViewWidth");
	directionLight.viewHight = globalVariables->GetFloatValue(groupName, "DirectionLightViewHight");
	directionLight.nearClip = globalVariables->GetFloatValue(groupName, "DirectionLightNearClip");
	directionLight.farClip = globalVariables->GetFloatValue(groupName, "DirectionLightFarClip");


	camera_ = CameraManager::GetInstance()->GetCamera();
	world_ = std::make_unique<yunity::World>();
	world_->Initialize(gravity_);

	bottonSprite_ = std::make_unique<yunity::Sprite>();
	bottonSprite_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("ABotton.dds"), spritePos_));
	bottonPushSprite_ = std::make_unique<yunity::Sprite>();
	bottonPushSprite_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("ABottonPush.png"), spritePos_));
	time_ = 0;

	yunity::Model::DirectionalLight l = { .color = directionLight.color, .direction = directionLight.direction, .intensity = directionLight.intensity };
	directionLight_ = std::make_unique<yunity::DirectionLight>();
	directionLight_->Initialize(directionLight.eyePosition, directionLight.targetPosition, directionLight.upDirection, directionLight.viewWidth, directionLight.viewHight, directionLight.nearClip, directionLight.farClip);

	camera_->SetTarget(nullptr);

	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize();
	objectManager_->Load("title", camera_, world_.get());
	objectManager_->CreatePlayer(camera_, world_.get());
	objectManager_->SetDirectionalLight(l);
	objectManager_->SetDirectionLight(directionLight_.get());

	player_ = dynamic_cast<Player*>(objectManager_->GetObj(Tag::kPlayer).back());
	player_->SetMass(0.0f);

	skydome_ = std::make_unique<yunity::Skydome>();
	skydome_->Initialize(camera_, skydomeScale_);

	isStart_ = false;

	camera_->SetTranslate(cameraPos_);
	CommonData::GetInstance()->scene_ = Scene::kTitle;
	CommonData::GetInstance()->flagState_ = FlagState::kCount;

}

void TitleScene::Update()
{
	prePad_ = pad_;

	if (!Tradition::GetInstance()->GetIsActive()) {
		yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, false);
		if (isStart_) {
			player_->SetMass(playerMass_);
			CommonData::GetInstance()->flagState_ = FlagState::kPlay;
		}
		if (player_->GetWorldTransform()->GetMatWorldTranslation().x >= targetPoint_ && camera_->GetTarget() == nullptr) {
			camera_->SetTarget(player_->GetWorldTransform());

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
		if (!Tradition::GetInstance()->GetIn()) {
			yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, false);
			yunity::SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
		}
	}

	if (yunity::Input::GetInstance()->TriggerKey(DIK_0)) {
		yunity::SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
	}

	objectManager_->Update();

	world_->Solve();


	time_ += yunity::fixedTimeStep_;
	if (time_ > limitTime_) {
		time_ = 0;
		isDraw_ ^= true;

	}

	directionLight_->Update();

#ifdef _DEBUG


#endif

}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	skydome_->Draw();
	objectManager_->Draw();
}

void TitleScene::DrawFront()
{
	if ((player_->GetSelect() && CommonData::GetInstance()->flagState_ == FlagState::kPlay) || !isStart_) {
		if (isDraw_) {
			bottonSprite_->Draw();
		}
		else {
			bottonPushSprite_->Draw();
		}
	}
	Tradition::GetInstance()->Draw();
}

