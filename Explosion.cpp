#include "Explosion.h"

void Explosion::Initialize(Model* model, const Vector3& translation, const Vector3& velocity)
{
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = translation;
	worldTransform_.UpdateMatrix(RotationType::Euler);
	velocity_ = velocity;
	isDead_ = false;
	model_->SetMaterial(Vector4{ 1.0f, 1.0f, 1.0f, 0.4f });
}

void Explosion::Update()
{
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	if (worldTransform_.scale_.x <= 0.0f) {
		isDead_ = true;
	}
	else {
		worldTransform_.scale_ = Add(worldTransform_.scale_, Vector3{ -0.01f, -0.01f, -0.01f });
	}
	worldTransform_.UpdateMatrix(RotationType::Euler);
}

void Explosion::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

