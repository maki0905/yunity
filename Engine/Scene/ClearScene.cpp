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
	model0_.reset(ModelManager::GetInstance()->CreateModel(gltf, true, "human", "sneakWalk"));
	model0_->SetCamera(camera_.get());
	//model0_->PlayAnimation();
	worldTransform0_.Initialize();
	worldTransform0_.scale_ = Vector3(10.0f, 10.0f, 10.0f);
	//worldTransform0_.rotation_.y = 210.5f;
}

void ClearScene::Update()
{
	//worldTransform0_.rotation_.y += 0.05f;
	//worldTransform0_.quaternion_.y += 0.01f;
	//worldTransform0_.rotation_.y += 0.05f;
	//worldTransform0_.UpdateMatrix(RotationType::Euler);
	worldTransform0_.UpdateMatrix(RotationType::Quaternion);

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
	model0_->Draw(worldTransform0_, TextureManager::GetInstance()->Load("uvChecker.png"));
}

void ClearScene::DrawFront()
{
}
