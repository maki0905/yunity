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
	//world_ = std::make_unique<World>();
	//world_->Initialize();

	//camera_ = std::make_unique<Camera>();
	//debugCamera_ = std::make_unique<DebugCamera>();
	//isDebug_ = false;

	////player_ = std::make_unique<Player>();
	////player_->Initialize(camera_.get());

	//blockManager_ = std::make_unique<BlockManager>();
	//blockManager_->Initialize(camera_.get(), world_.get());

	//start_ = std::make_unique<Model>();
	//start_.reset(ModelManager::GetInstance()->CreateModel(obj, "startBox"));
	//start_->SetCamera(camera_.get());
	//worldTransform_start_.Initialize();
	//worldTransform_start_.translation_ = { -20.0f, 5.0f, 0.0f };
	//worldTransform_start_.UpdateMatrix(RotationType::Euler);

	//end_ = std::make_unique<Model>();
	//start_.reset(ModelManager::GetInstance()->CreateModel(obj, "startBox"));
	//end_->SetCamera(camera_.get());
	//worldTransform_end_.Initialize();
	//worldTransform_end_.translation_ = { 50.0f, 3.0f, 0.0f };
	//worldTransform_end_.UpdateMatrix(RotationType::Euler);

	//world_->Add(player_.get());

	////player_->SetTranslation(worldTransform_start_.translation_);

	//primitiveDrawer_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
	//primitiveDrawer_->SetCamera(camera_.get());

	/*---------------------------------------------------------*/

	//camera_ = std::make_unique<Camera>();
	//camera_.reset(CameraManager::GetInstance()->GetCamera());
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

	/*spike_ = std::make_unique<Object3D>();
	spike_->Initialize(ModelManager::GetInstance()->CreateModel(obj, "Spike"), world_.get(), Collider::Shape::kAABB);
	spike_->SetCamera(camera_);
	spike_->SetPosition({ 20.0f, 15.0f, 0.0f });
	spike_->SetMass(1.0f);
	spike_->SetIsTrigger(true);
	stand_ = std::make_unique<Object3D>();
	stand_->Initialize(ModelManager::GetInstance()->CreateModel(obj, "SpikeStand"), world_.get(), Collider::Shape::kAABB);
	stand_->SetCamera(camera_);
	stand_->SetPosition({ 20.0f, 5.0f, 0.0f });
	stand_->SetIsTrigger(true);
	world_->Add(spike_.get());
	world_->Add(stand_.get());

	springJoint_ = std::make_unique<SpringJoint>();
	springJoint_->CreateSpringJoint(spike_.get(), stand_.get());
	springJoint_->EnableSpring(1, true);
	springJoint_->SetEquilibriumPoint(1, 10.0f);
	springJoint_->SetStiffness(1, 1.0f);
	springJoint_->SetDamping(1, 0.0f);
	world_->AddJoint(springJoint_.get());

	line_ = std::make_unique<PrimitiveDrawer>();
	line_.reset(PrimitiveDrawer::Create());
	line_->SetCamera(camera_);*/

	isReset_ = false;
	isDebt_ = false;
	resetTime_ = 0.0f;
	sizeTime_ = 0.0f;
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
