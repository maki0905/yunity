#include "Skydome.h"

#include "ModelManager.h"

void Skydome::Initialize(Camera* camera, Vector3 size)
{
	model_ = new Model();
	model_ = ModelManager::GetInstance()->CreateModel(obj, "skydome");
	model_->SetCamera(camera);

	worldTransform_.Initialize();
	worldTransform_.scale_ = size;
	worldTransform_.UpdateMatrix();
}

void Skydome::Update()
{
}

void Skydome::Draw()
{
	model_->Draw(worldTransform_);
}
