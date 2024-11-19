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
	debugCamera_ = std::make_unique<DebugCamera>();
	isDebug_ = false;

	skyboxWorldTransform_.Initialize();
	skyboxWorldTransform_.scale_ = { 100.0f, 100.0f, 100.0f };
	skybox_ = std::make_unique<SkyBox>();
	skybox_.reset(SkyBox::Create());
	skybox_->SetCamera(camera_);
	skybox_->SetTexture("rostock_laage_airport_4k.dds");

	Model::DirectionalLight l = { .color = {1.0f, 1.0f, 1.0f, 1.0f}, .direction = {1.0f, -1.0f, 0.0f}, .intensity = 1.0f };

	world_ = std::make_unique<World>();
	world_->Initialize({0.0f, -15.0f, 0.0f});

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_, world_.get());
	player_->SetDirectionalLight(l);
	camera_->SetTarget(player_->GetWorldTransform());

	startWT_.Initialize();

	stageName_ = CommonData::GetInstance()->GetStageName();
	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize();
	objectManager_->Load(stageName_, camera_, world_.get());
	objectManager_->SetDirectionalLight(l);
	startWT_.translation_ = objectManager_->GetPos("startBox");
	startPos_ = startWT_.translation_;
	player_->ResetPos(startPos_);

	start_ = std::make_unique<Model>();
	start_.reset(ModelManager::GetInstance()->CreateModel(obj, "TV"));
	start_->SetCamera(camera_);
	startWT_.translation_.y += 1.0f;
	startWT_.translation_.z += 3.0f;
	startWT_.scale_ = { 0.5f, 0.5f, 0.5f };
	startWT_.UpdateMatrix();
	textureTV_ = TextureManager::Load("Models/TV/TV.png");

	end_ = std::make_unique<Model>();
	end_.reset(ModelManager::GetInstance()->CreateModel(obj, "TV"));
	end_->SetCamera(camera_);
	endWT_.Initialize();
	endWT_.translation_ = endPos_;
	endWT_.scale_ = { 1.5f, 1.5f, 1.5f };
	endWT_.UpdateMatrix();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(camera_, {5.0f, 5.0f, 5.0f});

	CommonData::GetInstance()->scene_ = Scene::kStage;

	isReset_ = false;
	isDebt_ = false;
	resetTime_ = 0.0f;
	sizeTime_ = 0.0f;

	for (uint32_t i = 0; i < springBoardCount_; i++) {
		springTops_[i] = std::make_unique<SpringBoard>();
		springAnchors_[i] = std::make_unique<Object3D>();
		Vector3 pos;
		if (i == 0) {
			pos = {103.0f, 2.5f, 0.0f};
		}
		else {
			pos = {257.6f, -4.5f, -1.0f};
		}

		springTops_[i]->Initialize(ModelManager::GetInstance()->CreateModel(obj, "Wood"), world_.get(), Collider::kAABB);
		springTops_[i]->SetTranslation(pos);
		springTops_[i]->SetMass(1.0f);
		springTops_[i]->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		springTops_[i]->SetScale({ 6.5f, 1.0f, 8.5f });
		springTops_[i]->SetCamera(camera_);
		springTops_[i]->SetCollisionAttribute(kCollisionAttributeTrampoline);
		springTops_[i]->SetFixedPosition(pos);
		springTops_[i]->SetDirectionalLight(l);
		world_->Add(springTops_[i].get());

		springAnchors_[i]->Initialize(world_.get(), Collider::kAABB);
		springAnchors_[i]->SetTranslation({ pos.x, pos.y - 1.0f, pos.z });
		springAnchors_[i]->SetCamera(camera_);
		springAnchors_[i]->SetCollisionAttribute(kCollisionAttributeFloor);
		world_->Add(springAnchors_[i].get());

		springTops_[i]->SetSpringJoint(springAnchors_[i].get());

		springLines_[i] = std::make_unique<PrimitiveDrawer>();
		springLines_[i].reset(PrimitiveDrawer::Create());
		springLines_[i]->SetCamera(camera_);
	}

	for (uint32_t i = 0; i < bridgeCount_; i++) {
		bridge_[i] = std::make_unique<Object3D>();
		bridge_[i]->SetPosition(Vector3{ 397.45f + i * 15.0f,  26.765f, 0.0f });
		if (i > 0 && i < bridgeCount_ - 1) {
			bridge_[i]->SetMass(2.0f);
		}
		bridge_[i]->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Wood"), world_.get(), Collider::Shape::kAABB);
		bridge_[i]->SetCamera(camera_);
		bridge_[i]->SetScale({ 3.0f, 2.0f ,6.4f });
		bridge_[i]->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		bridge_[i]->SetCollisionAttribute(kCollisionAttributeTrampoline);
		bridge_[i]->SetDirectionalLight(l);
		world_->Add(bridge_[i].get());



		if (i > 0) {
			bridgesJoint_[i - 1] = std::make_unique<SpringJoint>();
			bridgesJoint_[i - 1]->CreateSpringJoint(bridge_[i - 1].get(), bridge_[i].get());
			bridgesJoint_[i - 1]->EnableSpring(0, true);
			bridgesJoint_[i - 1]->EnableSpring(1, true);
			bridgesJoint_[i - 1]->SetEquilibriumPoint(0, 0.0f);
			bridgesJoint_[i - 1]->SetEquilibriumPoint(1, 0.0f);
			bridgesJoint_[i - 1]->SetStiffness(0, stiffness_);
			bridgesJoint_[i - 1]->SetStiffness(1, stiffness_);
			bridgesJoint_[i - 1]->SetDamping(0, damper_);
			bridgesJoint_[i - 1]->SetDamping(1, damper_);
			world_->AddJoint(bridgesJoint_[i - 1].get());

			bridgeLines_[i - 1] = std::make_unique<PrimitiveDrawer>();
			bridgeLines_[i - 1].reset(PrimitiveDrawer::Create());
			bridgeLines_[i - 1]->SetCamera(camera_);
		}

	}
	
}

void StageScene::Update()
{

	if (!inStage_) {
		if (!Tradition::GetInstance()->GetOut()) {
			inStage_ = true;
		}
		Tradition::GetInstance()->Update();
		camera_->SetOffset(Lerp({0.0f, 0.0f, 0.0f}/*startPos_*/, { 0.0/*startPos_.x*/, 5.0f, -50.0f }, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
		player_->SetScale(Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
		if (player_->GetWorldTransform()->scale_.x == 1.0f) {
			//camera_->SetTarget(player_->GetWorldTransform());
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
			camera_->SetTranslate(Lerp(dieCamera_, { startPos_.x, startPos_.y + camera_->GetOffset().y, dieCamera_.z}, resetTime_));
			player_->SetTranslation(Lerp(diePos_, startPos_, resetTime_));
			if (resetTime_ == 1.0f) {
				player_->ResetPos(startPos_);
				player_->SetIsTrigger(false);
				camera_->SetTarget(player_->GetWorldTransform());
			}
		}
	}
	
	world_->Solve();

	for (uint32_t i = 0; i < springBoardCount_; i++) {
		springTops_[i]->Update();
	}

	skyboxWorldTransform_.UpdateMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
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
			player_->AddForce({0.0f, 10.0f, -1.0f}, Body::ForceMode::kImpulse);
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
			player_->SetPosition(Lerp(playerPos_, { endPos_.x, playerPos_.y, 0.0f }, time_));
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
				camera_->SetTranslate(Lerp(cameraPos_, { cameraPos_.x, endPos_.y, -60.0f }, time_));
				if (time_ == 1.0f) {
					isClear_ = true;
					time_ = 0.0f;
					cameraPos_ = camera_->GetTranslate();
					RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, true);
				}
			}
			
			/*time_ += 1.0f / 30.0f;
			time_ = std::clamp(time_, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(cameraPos_, { cameraPos_.x, endPos_.y, -60.0f }, time_));
			if (time_ == 1.0f) {
				if (player_->Result()) {
					isClear_ = true;
					time_ = 0.0f;
					cameraPos_ = camera_->GetTranslate();
					RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, true);
				}
			}*/

		}
		else {
			time_ += 1.0f / 60.0f;
			time_ = std::clamp(time_, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(cameraPos_, endPos_, time_));
			Tradition::GetInstance()->Update();
			

			if (!Tradition::GetInstance()->GetIn()) {
				CommonData::GetInstance()->isGoal_ = false;
				SceneManager::GetInstance()->ChangeScene("TITLE");
				player_->SetSelect(false);
				Tradition::GetInstance()->Initialize();
			}
		}


	}

	if (Input::GetInstance()->TriggerKey(DIK_T)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
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
	for (uint32_t i = 0; i < springBoardCount_; i++) {
		springLines_[i]->Draw(springTops_[i]->GetMatWorldTranslation(), springAnchors_[i]->GetMatWorldTranslation(), { 0.0f, 0.0f, 0.0f, 1.0f });
		springTops_[i]->Draw();
	}
	for (uint32_t i = 0; i < bridgeCount_; i++) {
		if (i > 0) {
			bridgeLines_[i - 1]->Draw(bridge_[i - 1]->GetMatWorldTranslation(), bridge_[i]->GetMatWorldTranslation(), { 0.0f, 0.0f, 0.0f, 1.0f });
		}
		bridge_[i]->Draw();
	}
	start_->Draw(startWT_, textureTV_);
	end_->Draw(endWT_, textureTV_);
	player_->Draw();
}

void StageScene::DrawFront()
{
	player_->DrawUI();
	Tradition::GetInstance()->Draw();
}
