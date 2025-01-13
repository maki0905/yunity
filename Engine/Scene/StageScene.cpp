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
	directionLight_->Initialize({ 0.0f, 40.0f, 0.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, 40.0f, 40.0f, 0.0f, 20.0f);

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_, world_.get());
	player_->SetDirectionalLight(l);
	player_->SetDirectionLight(directionLight_.get());
	camera_->SetTarget(player_->GetWorldTransform());

	startWT_.Initialize();

	//stageName_ = CommonData::GetInstance()->GetStageName();
	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize(camera_);
	objectManager_->Load(stageName_, camera_, world_.get(), "joints_data");
	objectManager_->SetDirectionalLight(l);
	objectManager_->SetDirectionLight(directionLight_.get());
	startWT_.translation_ = objectManager_->GetPos("startBox");
	startPos_ = startWT_.translation_;
	//startPos_ = { 730.0f, 63.0f, 0.0f };
	player_->Reset(startPos_);

	start_ = std::make_unique<yunity::Model>();
	start_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "TV"));
	start_->SetCamera(camera_);
	startWT_.translation_.y += 1.0f;
	startWT_.translation_.z += 3.0f;
	startWT_.scale_ = { 0.5f, 0.5f, 0.5f };
	startWT_.UpdateMatrix();
	textureTV_ = yunity::TextureManager::Load("Models/TV/TV.png");

	end_ = std::make_unique<yunity::Model>();
	end_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "TV"));
	end_->SetCamera(camera_);
	endWT_.Initialize();
	endWT_.translation_ = endConstant_.pos;
	endWT_.scale_ = { 1.5f, 1.5f, 1.5f };
	endWT_.UpdateMatrix();

	skydome_ = std::make_unique<yunity::Skydome>();
	skydome_->Initialize(camera_, { 10.0f, 10.0f, 10.0f });

	CommonData::GetInstance()->scene_ = Scene::kStage;

	isReset_ = false;
	isDebt_ = false;
	resetTime_ = 0.0f;
	sizeTime_ = 0.0f;
}

void StageScene::Update()
{

	if (!inStage_) {
		if (!Tradition::GetInstance()->GetOut()) {
			inStage_ = true;
		}
		Tradition::GetInstance()->Update();
		camera_->SetOffset(Lerp({ 0.0f, 0.0f, 0.0f }, { 0.0, 5.0f, -50.0f }, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
		player_->SetScale(Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
		if (player_->GetWorldTransform()->scale_.x == 1.0f) {
			player_->SetDisplayUI(true, Player::UI::kScore);
			player_->SetDisplayUI(true, Player::UI::kReticle);
		}
	}
	else {
		if (!isReset_) {
			player_->Update();
			objectManager_->Update();

		}

	}

	if (isReset_) {
		if (resetTime_ == 1.0f) {

			sizeTime_ += 1.0f / 60.0f;
			sizeTime_ = std::clamp(sizeTime_, 0.0f, 1.0f);
			player_->SetScale(Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, sizeTime_));

			if (sizeTime_ == 1.0f) {
				isReset_ = false;
				isDebt_ = false;
				resetTime_ = 0.0f;
				sizeTime_ = 0.0f;
				upTime_ = 0.0f;
				downTime_ = 0.0f;
			}

		}
		else {
			resetTime_ += 1.0f / 30.0f;
			resetTime_ = std::clamp(resetTime_, 0.0f, 1.0f);
			Vector3 spawnPoint = player_->GetSpawnPoint();
			camera_->SetTranslate(Lerp(dieCamera_, { spawnPoint.x, spawnPoint.y + camera_->GetOffset().y, dieCamera_.z}, resetTime_));
			player_->SetTranslation(Lerp(diePos_, spawnPoint, resetTime_));
			if (resetTime_ == 1.0f) {
				player_->Reset();
				player_->SetIsTrigger(false);
				camera_->SetTarget(player_->GetWorldTransform());
			}
		}
	}

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

	if (!player_->GetActive()) {

		if (!isDebt_) {
			dieCamera_ = camera_->GetTranslate();
			diePos_ = player_->GetMatWorldTranslation();
			camera_->SetTarget(nullptr);
			player_->AddForce({ 0.0f, 10.0f, -1.0f }, yunity::Body::ForceMode::kImpulse);
			player_->SetIsTrigger(true);
			isDebt_ = true;
			player_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}
		else {
			if (upTime_ < 1.0f) {
				upTime_ += 1.0f / 15.0f;
				upTime_ = std::clamp(upTime_, 0.0f, 1.0f);
				float t = 1.0f - std::powf(1.0f - upTime_, 5);
				Vector3 pos = Slerp(diePos_, { diePos_.x, diePos_.y + 10.0f, diePos_.z - 10.0f }, t);
				pos.x = diePos_.x;
				player_->SetTranslation(pos);
				if (upTime_ == 1.0f) {
					topPos_ = player_->GetTranslation();
				}
			}
			else if (downTime_ < 1.0f) {
				downTime_ += 1.0f / 30.0f;
				downTime_ = std::clamp(downTime_, 0.0f, 1.0f);
				Vector3 pos = Slerp(topPos_, { topPos_.x, topPos_.y - 20.0f, topPos_.z - 10.0f }, downTime_);
				pos.x = topPos_.x;
				player_->SetTranslation(pos);
			}
			else {
				isReset_ = true;
				diePos_ = player_->GetTranslation();
				player_->SetIsTrigger(false);
				player_->SetScale({ 0.0f, 0.0f, 0.0f });
			}
		}
	}

	if (CommonData::GetInstance()->isGoal_) {
		if (!isGoal_) {
			if (Length(playerPos_) == 0.0f) {
				playerPos_ = player_->GetMatWorldTranslation();
			}
			time_ += 1.0f / 120.0f;
			time_ = std::clamp(time_, 0.0f, 1.0f);
			player_->SetPosition(Lerp(playerPos_, { endConstant_.pos.x, playerPos_.y, 0.0f }, time_));


			if (time_ == 1.0f) {
				isGoal_ = true;
				cameraPos_ = camera_->GetTranslate();
				camera_->SetTarget(nullptr);
				Tradition::GetInstance()->Initialize();
				Tradition::GetInstance()->Start();
				time_ = 0.0f;
			}

		}
		else if (!isClear_) {
			if (player_->Result()) {
				time_ += 1.0f / 30.0f;
				time_ = std::clamp(time_, 0.0f, 1.0f);
				camera_->SetTranslate(Lerp(cameraPos_, { cameraPos_.x, endConstant_.pos.y, -60.0f }, time_));
				if (time_ == 1.0f) {
					isClear_ = true;
					time_ = 0.0f;
					cameraPos_ = camera_->GetTranslate();
					yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, true);
				}
			}

		}
		else {
			time_ += 1.0f / 60.0f;
			time_ = std::clamp(time_, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(cameraPos_, endConstant_.pos, time_));
			Tradition::GetInstance()->Update();


			if (!Tradition::GetInstance()->GetIn()) {
				CommonData::GetInstance()->isGoal_ = false;
				yunity::SceneManager::GetInstance()->ChangeScene("TITLE");
				player_->SetSelect(false);
				Tradition::GetInstance()->Initialize();
			}
		}


	}

	if (yunity::Input::GetInstance()->TriggerKey(DIK_T)) {
		yunity::SceneManager::GetInstance()->ChangeScene("TITLE");
	}



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
	start_->Draw(startWT_, textureTV_);
	end_->Draw(endWT_, textureTV_);
	player_->Draw();
}

void StageScene::DrawFront()
{
	player_->DrawUI();
	Tradition::GetInstance()->Draw();
}
