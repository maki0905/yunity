#include "ClearScene.h"

void ClearScene::Initialize()
{
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();
	bool isDebug_ = false;

	model0_ = std::make_unique<Model>();
	model0_.reset(Model::Create("terrain"));
	model0_->SetCamera(camera_.get());
	worldTransform0_.Initialize();


}

void ClearScene::Update()
{
	worldTransform0_.UpdateMatrix(RotationType::Euler);

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
}

void ClearScene::DrawFront()
{
}
