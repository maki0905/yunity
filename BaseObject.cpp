#include "BaseObject.h"

void BaseObject::Initialize(const std::vector<Model*>& models)
{
	models_ = models;
	worldTransform_.Initialize();
}

void BaseObject::Update()
{
	worldTransform_.UpdateMatrix();
}

void BaseObject::Draw(const ViewProjection& viewProjection)
{
	// モデル描画
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
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

