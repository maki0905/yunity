#include "Skydome.h"

void Skydome::Initialize(Model* model, Camera* camera)
{
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

	collider_ = new Collider();
	collider_->CreateCollider(&worldTransform_, Collider::Type::kSphere, RotationType::Euler, camera);
}

void Skydome::Update()
{
}

void Skydome::Draw(const Camera& camera)
{
	model_->Draw(worldTransform_);
}
