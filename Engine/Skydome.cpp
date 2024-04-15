#include "Skydome.h"

void Skydome::Initialize(Camera* camera)
{

	model_.reset(Model::Create("skydome"));
	model_->SetCamera(camera);
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix(RotationType::Euler);
}

void Skydome::Update()
{
}

void Skydome::Draw()
{
	model_->Draw(worldTransform_);
}
