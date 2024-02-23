#include "Skydome.h"

void Skydome::Initialize(Model* model)
{
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix(RotationType::Euler);

}

void Skydome::Update()
{
}

void Skydome::Draw(const Camera& camera)
{
	model_->Draw(worldTransform_);
}
