#include "ClearScene.h"

#include "ModelManager.h"

void ClearScene::Initialize()
{
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();
	bool isDebug_ = false;

	model0_ = std::make_unique<Model>();
	//model0_.reset(Model::Create("terrain", "obj"));
	//model0_.reset(Model::Create("plane", "gltf"));
	//model0_.reset(ModelManager::GetInstance()->CreateModel(gltf, true, "simpleSkin"));
	model0_.reset(ModelManager::GetInstance()->CreateModel(gltf, "human", "sneakWalk", ModelType::kSkin));
	model0_->SetCamera(camera_.get());
	worldTransform0_.Initialize();
	worldTransform0_.scale_ = Vector3(10.0f, 10.0f, 10.0f);
	worldTransform0_.translation_ = { 10.0f, 0.0f, 0.0f };
	worldTransform0_.quaternion_ = { 0.0f, 1.0f, 0.0f, 0.0f };

	model1_ = std::make_unique<Model>();
	model1_.reset(ModelManager::GetInstance()->CreateModel(gltf, "human", "walk", ModelType::kSkin));
	model1_->SetCamera(camera_.get());
	worldTransform1_.Initialize();
	worldTransform1_.scale_ = Vector3(10.0f, 10.0f, 10.0f);
	worldTransform1_.translation_ = { -10.0f, 0.0f, 0.0f };
	worldTransform1_.quaternion_ = { 0.0f, 1.0f, 0.0f, 0.0f };

	model2_ = std::make_unique<Model>();
	model2_.reset(ModelManager::GetInstance()->CreateModel(obj, "terrain"));
	model2_->SetCamera(camera_.get());
	worldTransform2_.Initialize();
	//worldTransform2_.scale_ = { 10.0f, 10.0f, 10.0f };
	
}

void ClearScene::Update()
{
	//worldTransform0_.rotation_.y += 0.05f;
	//worldTransform0_.quaternion_.y += 0.01f;
	//worldTransform0_.rotation_.y += 0.05f;
	//worldTransform0_.UpdateMatrix(RotationType::Euler);
	worldTransform0_.UpdateMatrix(RotationType::Quaternion);
	worldTransform1_.UpdateMatrix(RotationType::Quaternion);
	worldTransform2_.UpdateMatrix(RotationType::Euler);

	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		model0_->PlayAnimation();
	}
	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		model1_->PlayAnimation();
	}


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
}

void ClearScene::DrawBack()
{
}

void ClearScene::Draw3D()
{
	model0_->Draw(worldTransform0_);
	model1_->Draw(worldTransform1_);
	model2_->Draw(worldTransform2_);
}

void ClearScene::DrawFront()
{
}
