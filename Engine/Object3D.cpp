#include "Object3D.h"

void Object3D::Create(Model* model)
{
	model_ = std::make_unique<Model>();
	model_.reset(model);
	worldTransform_.Initialize();
}

void Object3D::Update()
{
	worldTransform_.UpdateMatrix();
}

void Object3D::Draw()
{
	model_->Draw(worldTransform_);
}
