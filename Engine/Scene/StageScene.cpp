#include "StageScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "CommonData.h"
#include "Tradition.h"

void StageScene::Initialize()
{
	
	inStage_ = false;

	camera_ = CameraManager::GetInstance()->GetCamera();
	camera_->SetFixedAxis({ 0.0f, 0.0f, 1.0f });
	debugCamera_ = std::make_unique<DebugCamera>();
	isDebug_ = false;

	skyboxWorldTransform_.Initialize();
	skyboxWorldTransform_.scale_ = { 100.0f, 100.0f, 100.0f };
	skybox_ = std::make_unique<SkyBox>();
	skybox_.reset(SkyBox::Create());
	skybox_->SetCamera(camera_/*camera_.get()*/);
	skybox_->SetTexture("rostock_laage_airport_4k.dds");

	world_ = std::make_unique<World>();
	world_->Initialize({0.0f, -15.0f, 0.0f});
	//world_->Initialize();

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_/*camera_.get()*/, world_.get());
	camera_->SetTarget(player_->GetWorldTransform());

	//start_ = std::make_unique<Model>();
	////start_.reset(ModelManager::GetInstance()->CreateModel(obj, "startBox"));
	//start_.reset(ModelManager::GetInstance()->CreateModel(obj, "Cube"));
	//start_->SetCamera(camera_.get());
	startWT_.Initialize();
	//startWT_.translation_.y = 6.0f;
	//end_ = std::make_unique<Model>();
	//end_.reset(ModelManager::GetInstance()->CreateModel(obj, "endBox"));
	//end_->SetCamera(camera_.get());
	//endWT_.Initialize();
	//endWT_.translation_ = { 10.0f, 3.0f, 0.0f };

	stageName_ = CommonData::GetInstance()->GetStageName();
	//ObjectManager::GetInstance()->Load(stageName_, camera_/*camera_.get()*/, world_.get());
	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize();
	objectManager_->Load(stageName_, camera_/*camera_.get()*/, world_.get());
	startWT_.translation_ = objectManager_->GetPos("startBox");
	//startWT_.translation_ = ObjectManager::GetInstance()->GetPos(stageName_, "startBox");
	startPos_ = startWT_.translation_;
	player_->ResetPos(startPos_);
	//ObjectManager::GetInstance()->Load("TL1", camera_.get(), world_.get());
	/*trampolines_ = ObjectManager::GetInstance()->GetObjects("stage0", "Trampoline");*/

	start_ = std::make_unique<Model>();
	start_.reset(ModelManager::GetInstance()->CreateModel(obj, "TV"));
	start_->SetCamera(camera_);
	startWT_.translation_.y += 1.0f;
	startWT_.translation_.z += 3.0f;
	startWT_.scale_ = { 0.5f, 0.5f, 0.5f };
	startWT_.UpdateMatrix();
	textureTV_ = TextureManager::Load("Models/TV/TV.png");

	//ObjectManager::GetInstance()->SetActive(stageName_, 0, false);

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(camera_/*camera_.get()*/, {5.0f, 5.0f, 5.0f});

	CommonData::GetInstance()->scene_ = Scene::kStage;

	isReset_ = false;
	isDebt_ = false;
	resetTime_ = 0.0f;
	sizeTime_ = 0.0f;

	for (uint32_t i = 0; i < springBoardCount_; i++) {
		springTops_[i] = std::make_unique<SpringBoard>();
		springAnchores_[i] = std::make_unique<Object3D>();
		Vector3 pos;
		if (i == 0) {
			pos = {103.0f, 2.5f, 0.0f};
		}
		else {
			pos = {257.6f, -4.5f, 0.0f};
		}

		springTops_[i]->Initialize(ModelManager::GetInstance()->CreateModel(obj, "Wood"), world_.get(), Collider::kAABB);
		springTops_[i]->SetTranslation(pos);
		springTops_[i]->SetMass(1.0f);
		springTops_[i]->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		springTops_[i]->SetScale({ 2.5f, 1.0f, 3.5f });
		springTops_[i]->SetCamera(camera_);
		springTops_[i]->SetCollisionAttribute(kCollisionAttributeTrampoline);
		springTops_[i]->SetFixedPosition(pos);
		world_->Add(springTops_[i].get());

		springAnchores_[i]->Initialize(world_.get(), Collider::kAABB);
		springAnchores_[i]->SetTranslation({ pos.x, pos.y - 1.0f, pos.z });
		springAnchores_[i]->SetCamera(camera_);
		springAnchores_[i]->SetCollisionAttribute(kCollisionAttributeFloor);
		world_->Add(springAnchores_[i].get());

		springTops_[i]->SetSpringJoint(springAnchores_[i].get());

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
			bridgesJoint_[i - 1]->SetDamping(0, dampar_);
			bridgesJoint_[i - 1]->SetDamping(1, dampar_);
			world_->AddJoint(bridgesJoint_[i - 1].get());

			bridgeLines_[i - 1] = std::make_unique<PrimitiveDrawer>();
			bridgeLines_[i - 1].reset(PrimitiveDrawer::Create());
			bridgeLines_[i - 1]->SetCamera(camera_);
		}

	}
	
}

void StageScene::Update()
{
	//player_->Update();

	///*if (player_->GetTranslation().x > worldTransform_end_.translation_.x) {
	//	player_->SetTranslation(worldTransform_start_.translation_);
	//}*/

	//blockManager_->Update(world_.get());

	//world_->Solve();

	//if (isDebug_) {
	//	debugCamera_->Update(camera_.get());
	//	camera_->Update();
	//}
	//else {
	//	camera_->Update();
	//}

	/*---------------------------------------------------------*/

	if (!inStage_) {
		if (!Tradition::GetInstance()->GetOut()) {
			inStage_ = true;
			//camera_->SetTarget(player_->GetWorldTransform());
		}
		Tradition::GetInstance()->Update();
		camera_->SetOffset(Lerp(startPos_, { startPos_.x, 5.0f, -50.0f }, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
		player_->SetScale(Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
		if (player_->GetWorldTransform()->scale_.Length() == 1.0f) {
			camera_->SetTarget(player_->GetWorldTransform());
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
	

	//ObjectManager::GetInstance()->Update(stageName_);
	///*for (auto& object : ObjectManager::GetInstance()->GetObjects("TL1")) {
	//	object->Update();
	//}*/
	world_->Solve();

	for (uint32_t i = 0; i < springBoardCount_; i++) {
		springTops_[i]->Update();
	}

	skyboxWorldTransform_.UpdateMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
	}

	if (isDebug_) {
		debugCamera_->Update(camera_/*camera_.get()*/);
		camera_->SetTarget(nullptr);
		camera_->Update();
	}
	else {
		/*if (camera_->GetTarget() == nullptr) {
			camera_->SetTarget(player_->GetWorldTransform());
		}*/
		camera_->Update();
	}

	/*startWT_.UpdateMatrix();
	endWT_.UpdateMatrix();*/

	if (!player_->GetActive()) {

		if (!isDebt_) {
			dieCamera_ = camera_->GetTranslate();
			diePos_ = player_->GetMatWorldTranslation();
			camera_->SetTarget(nullptr);
			player_->AddForce({0.0f, 10.0f, -1.0f}, 1);
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
				//player_->SetTranslation(Slerp(diePos_, { diePos_.x, diePos_.y + 10.0f, diePos_.z - 2.0f }, upTime_));
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
				//player_->SetTranslation(Slerp(topPos_, { topPos_.x, topPos_.y - 20.0f, topPos_.z - 2.0f }, downTime_));
				player_->SetTranslation(pos);
			}
			else {
				isReset_ = true;
				diePos_ = player_->GetTranslation();
				player_->SetIsTrigger(false);
				//player_->SetTranslation(startPos_);
				player_->SetScale({ 0.0f, 0.0f, 0.0f });
			}
		}

		/*if (player_->GetTranslation().y < -10.0f) {
			isReset_ = true;
			player_->SetIsTrigger(false);
		}*/
		//ObjectManager::GetInstance()->Clear(stageName_);
		//ObjectManager::GetInstance()->Load(stageName_, camera_/*camera_.get()*/, world_.get());
	}

	if (CommonData::GetInstance()->isGoal_) {
		CommonData::GetInstance()->isGoal_ = false;
		SceneManager::GetInstance()->ChangeScene("TITLE");


	}

	if (Input::GetInstance()->TriggerKey(DIK_T)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	

#ifdef _DEBUG
	/*Vector3 pos = spike_->GetMatWorldTranslation();
	ImGui::Begin("spike");
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::End();
	spike_->SetPosition(pos);*/

#endif


}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{
	/*player_->Draw();
	blockManager_->Draw();
	primitiveDrawer_->Draw(worldTransform_start_);
	start_->Draw(worldTransform_start_);
	end_->Draw(worldTransform_end_);*/
	//primitiveDrawer_->Draw(worldTransform_start_);

	/*---------------------------------------------------------*/

	//skybox_->Draw(skyboxWorldTransform_);
	//start_->Draw(startWT_);
	/*start_->Draw(startWT_);
	end_->Draw(endWT_);*/
	skydome_->Draw();
	objectManager_->Draw();
	for (uint32_t i = 0; i < springBoardCount_; i++) {
		springLines_[i]->Draw(springTops_[i]->GetMatWorldTranslation(), springAnchores_[i]->GetMatWorldTranslation(), { 0.0f, 0.0f, 0.0f, 1.0f });
		springTops_[i]->Draw();
	}
	for (uint32_t i = 0; i < bridgeCount_; i++) {
		if (i > 0) {
			bridgeLines_[i - 1]->Draw(bridge_[i - 1]->GetMatWorldTranslation(), bridge_[i]->GetMatWorldTranslation(), { 0.0f, 0.0f, 0.0f, 1.0f });
		}
		bridge_[i]->Draw();
	}
	start_->Draw(startWT_, textureTV_);
	//ObjectManager::GetInstance()->Draw(stageName_);
	///*for (auto& object : ObjectManager::GetInstance()->GetObjects("TL1")) {
	//	object->Draw();
	//}*/
	/*line_->Draw(spike_->GetMatWorldTranslation(), stand_->GetMatWorldTranslation(), {0.0f, 0.0f, 0.0f, 1.0f});
	spike_->Draw();
	stand_->Draw();*/
	player_->Draw();
	

	//player_->Draw();
}

void StageScene::DrawFront()
{
	player_->DrawUI();
	Tradition::GetInstance()->Draw();
}
