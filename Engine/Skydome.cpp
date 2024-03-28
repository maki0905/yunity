#include "Skydome.h"

void Skydome::Initialize(Model* model, Camera* camera)
{
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix(RotationType::Euler);

	collider_ = new Collider();
	collider_->Create(&worldTransform_, Collider::Type::kSphere, RotationType::Euler, camera);
}

void Skydome::Update()
{
}

void Skydome::Draw(const Camera& camera)
{
	model_->Draw(worldTransform_);
}
