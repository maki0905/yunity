#include "Skydome.h"

#include "ModelManager.h"

void yunity::Skydome::Initialize(Camera* camera, Vector3 size)
{
	model_ = std::make_unique<Model>();
	model_.reset(ModelManager::GetInstance()->CreateModel(obj, "skydome"));
	model_->SetCamera(camera);

	worldTransform_.Initialize();
	worldTransform_.scale_ = size;
	worldTransform_.UpdateMatrix();
}

void yunity::Skydome::Update()
{
}

void yunity::Skydome::Draw()
{
	model_->Draw(worldTransform_);
}
