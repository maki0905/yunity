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

void StageScene::Initialize()
{

	inStage_ = false;

	isGoal_ = false;
	isClear_ = false;
	playerPos_ = { 0.0f, 0.0f, 0.0f };
	cameraPos_ = { 0.0f, 0.0f, 0.0f };

	camera_ = CameraManager::GetInstance()->GetCamera();
	camera_->SetFixedAxis({ 0.0f, 0.0f, 1.0f });
	debugCamera_ = std::make_unique<yunity::DebugCamera>();
	isDebug_ = false;

	DirectionLight directionLight;
	yunity::Model::DirectionalLight l = { .color = directionLight.color, .direction = directionLight.direction, .intensity = directionLight.intensity };

	world_ = std::make_unique<yunity::World>();
	world_->Initialize(gravity_);

	directionLight_ = std::make_unique<yunity::DirectionLight>();
	directionLight_->Initialize(directionLight.eyePosition, directionLight.targetPosition, directionLight.upDirection, directionLight.viewWidth, directionLight.viewHight, directionLight.nearClip, directionLight.farClip);

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_, world_.get());
	player_->SetDirectionalLight(l);
	player_->SetDirectionLight(directionLight_.get());
	camera_->SetTarget(player_->GetWorldTransform());

	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize(camera_);
	objectManager_->Load(stageName_, camera_, world_.get(), "joints_data");
	objectManager_->SetDirectionalLight(l);
	objectManager_->SetDirectionLight(directionLight_.get());
	player_->Reset(CommonData::GetInstance()->start_.GetMatWorldTranslation());

	skydome_ = std::make_unique<yunity::Skydome>();
	skydome_->Initialize(camera_, { 10.0f, 10.0f, 10.0f });

	CommonData::GetInstance()->scene_ = Scene::kStage;
	CommonData::GetInstance()->flagState_ = FlagState::kInStage;

	isReset_ = false;
	isDebt_ = false;
	resetTime_ = 0.0f;
	sizeTime_ = 0.0f;
}

void StageScene::Update()
{

	player_->Update();
	objectManager_->Update();

	world_->Solve();

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

	if (yunity::Input::GetInstance()->TriggerKey(DIK_T)) {
		yunity::SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	directionLight_->Update();


#ifdef _DEBUG


#endif


}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{

	skydome_->Draw();
	objectManager_->Draw();
	player_->Draw();
}

void StageScene::DrawFront()
{
	player_->DrawUI();
	Tradition::GetInstance()->Draw();
}
