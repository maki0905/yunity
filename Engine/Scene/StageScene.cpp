#include "StageScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "ModelManager.h"

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

	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();
	isDebug_ = false;

	skyboxWorldTransform_.Initialize();
	skyboxWorldTransform_.scale_ = { 100.0f, 100.0f, 100.0f };
	skybox_ = std::make_unique<SkyBox>();
	skybox_.reset(SkyBox::Create());
	skybox_->SetCamera(camera_.get());
	skybox_->SetTexture("rostock_laage_airport_4k.dds");

	world_ = std::make_unique<World>();
	world_->Initialize({0.0f, -9.8f, 0.0f});
	//world_->Initialize();

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_.get(), world_.get());

	start_ = std::make_unique<Model>();
	//start_.reset(ModelManager::GetInstance()->CreateModel(obj, "startBox"));
	start_.reset(ModelManager::GetInstance()->CreateModel(obj, "Cube"));
	start_->SetCamera(camera_.get());
	startWT_.Initialize();
	startWT_.translation_.y = 6.0f;
	end_ = std::make_unique<Model>();
	end_.reset(ModelManager::GetInstance()->CreateModel(obj, "endBox"));
	end_->SetCamera(camera_.get());
	endWT_.Initialize();
	endWT_.translation_ = { 10.0f, 3.0f, 0.0f };
	player_->ResetPos(startWT_.translation_);

	ObjectManager::GetInstance()->Load("stage2", camera_.get(), world_.get());
	//ObjectManager::GetInstance()->Load("TL1", camera_.get(), world_.get());
	/*trampolines_ = ObjectManager::GetInstance()->GetObjects("stage0", "Trampoline");*/

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(camera_.get(), {5.0f, 5.0f, 5.0f});

}

void StageScene::Update()
{
	//player_->Update();

	///*if (player_->GetTranslation().x > worldTransform_end_.translation_.x) {
	//	player_->SetTranslation(worldTransform_start_.translation_);
	//}*/

	//blockManager_->Update(world_.get());

	//world_->Solve();

	//if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
	//	isDebug_ ^= true;
	//}

	//if (isDebug_) {
	//	debugCamera_->Update(camera_.get());
	//	camera_->Update();
	//}
	//else {
	//	camera_->Update();
	//}

	/*---------------------------------------------------------*/


	player_->Update();
	for (auto& object : ObjectManager::GetInstance()->GetObjects("stage2")) {
		object->Update();
	}
	/*for (auto& object : ObjectManager::GetInstance()->GetObjects("TL1")) {
		object->Update();
	}*/
	world_->Solve();

	skyboxWorldTransform_.UpdateMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
	}

	if (isDebug_) {
		debugCamera_->Update(camera_.get());
		camera_->Update();
	}
	else {
		camera_->Update();
	}

	startWT_.UpdateMatrix();
	endWT_.UpdateMatrix();

	if (!player_->GetActive()) {
		player_->ResetPos(startWT_.translation_);
	}

	ImGui::Begin("end");
	ImGui::DragFloat3("translation", &endWT_.translation_.x);
	ImGui::End();

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
	for (auto& object : ObjectManager::GetInstance()->GetObjects("stage2")) {
		object->Draw();
	}
	/*for (auto& object : ObjectManager::GetInstance()->GetObjects("TL1")) {
		object->Draw();
	}*/
	player_->Draw();
	

	//player_->Draw();
}

void StageScene::DrawFront()
{
	player_->DrawUI();
}
