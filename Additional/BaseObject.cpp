#include "BaseObject.h"

void BaseObject::Initialize(const std::vector<Model*>& models)
{
	models_ = models;
	worldTransform_.Initialize();
}

void BaseObject::Update()
{
	worldTransform_.UpdateMatrix(RotationType::Euler);
}

void BaseObject::Draw(const Camera& camera)
{
	// モデル描画
	for (Model* model : models_) {
		model->Draw(worldTransform_, camera);
	}
}

void BaseObject::OnCollision(uint32_t collisionAttribute)
{
}

void BaseObject::OnCollision(WorldTransform* worldTransfrom)
{
}

Vector3 BaseObject::GetWorldPosition()
{
	return Vector3();
}

