#include "StartBox.h"

void StartBox::Initialize(Model* model)
{
	models_ = {model};
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 0.75f;
	worldTransform_.UpdateMatrix();
}

void StartBox::Update()
{
}

void StartBox::Draw(const ViewProjection& viewProjection)
{
	models_[0]->Draw(worldTransform_, viewProjection);
}
