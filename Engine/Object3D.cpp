#include "Object3D.h"

void Object3D::Create(Model* model, World* world, Collider::Type type)
{
	model_ = std::make_unique<Model>();
	model_.reset(model);
	worldTransform_.Initialize();
	CreateBody(world, &worldTransform_, 0.0f);
	CreateCollider(&worldTransform_, type, RotationType::Quaternion, model_->GetCamera(), size_);
}

void Object3D::Update()
{
	worldTransform_.UpdateMatrix();
}

void Object3D::Draw()
{
	model_->Draw(worldTransform_);
	Collider::HitBox();
}
