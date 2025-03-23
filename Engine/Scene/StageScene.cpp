#include "StageScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "CommonData.h"
#include "Tradition.h"
#include "RenderTexture.h"
#include "GlobalVariables.h"

void StageScene::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "StageScene";
	gravity_ = globalVariables->GetVector3Value(groupName, "Gravity");
	skydomeScale_ = globalVariables->GetVector3Value(groupName, "SkydomeScale");
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

	inStage_ = false;

	camera_ = CameraManager::GetInstance()->GetCamera();
	camera_->SetFixedAxis({ 0.0f, 0.0f, 1.0f });
	debugCamera_ = std::make_unique<yunity::DebugCamera>();
	isDebug_ = false;

	yunity::Model::DirectionalLight l = { .color = directionLight.color, .direction = directionLight.direction, .intensity = directionLight.intensity };

	world_ = std::make_unique<yunity::World>();
	world_->Initialize(gravity_);

	directionLight_ = std::make_unique<yunity::DirectionLight>();
	directionLight_->Initialize(directionLight.eyePosition, directionLight.targetPosition, directionLight.upDirection, directionLight.viewWidth, directionLight.viewHight, directionLight.nearClip, directionLight.farClip);

	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize();
	objectManager_->Load(stageName_, camera_, world_.get(), "joints_data");
	objectManager_->CreatePlayer(camera_, world_.get());
	objectManager_->SetDirectionalLight(l);
	objectManager_->SetDirectionLight(directionLight_.get());

	player_ = dynamic_cast<Player*>(objectManager_->GetObj(Tag::kPlayer).back());
	player_->Reset(CommonData::GetInstance()->start_.GetMatWorldTranslation());
	camera_->SetTarget(player_->GetWorldTransform());
	player_->ChangeProductionState(std::make_unique<InGameProduction>());

	skydome_ = std::make_unique<yunity::Skydome>();
	skydome_->Initialize(camera_, skydomeScale_);

	CommonData::GetInstance()->scene_ = Scene::kStage;
	CommonData::GetInstance()->flagState_ = FlagState::kInStage;
}

void StageScene::Update()
{
	objectManager_->Update();

	if (!player_->GetIsPose()) {
		world_->Solve();
	}

	directionLight_->Update();

#ifdef _DEBUG
	if (yunity::Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
		if (camera_->GetTarget() == nullptr) {
			camera_->SetTarget(player_->GetWorldTransform());
		}
		else {
			camera_->SetTarget(nullptr);
		}
	}

	if (isDebug_) {
		debugCamera_->Update(camera_);
		camera_->SetTarget(nullptr);
		camera_->Update();
	}
	else {
		camera_->Update();
	}


#endif


}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{
	skydome_->Draw();
	objectManager_->Draw();
}

void StageScene::DrawFront()
{
	player_->DrawUI();
	Tradition::GetInstance()->Draw();
}
