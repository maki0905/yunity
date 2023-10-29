#include "MovingFloor.h"

void MovingFloor::Initialize(const std::vector<Model*>& models)
{
	BaseObject::Initialize(models);
	worldTransform_.UpdateMatrix();
	// Õ“Ë‘®«‚ðÝ’è
	SetCollisionAttribute(kCollisionAttributeMovingFloor);
	// Õ“Ë‘ÎÛ‚ðŽ©•ª‚Ì‘®«ˆÈŠO‚ÉÝ’è
	SetCollisionMask(~kCollisionAttributeMovingFloor);

	size_ = { 15.0f, 0.5f, 15.0f };
}

void MovingFloor::Update()
{
	if (Length(Subtract(worldTransform_.translation_, startPosition_)) > 20.0f) {
		velocity_ = Multiply(-1.0f, velocity_);
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	aabb_.min = Subtract(worldTransform_.translation_, size_);
	aabb_.max = Add(worldTransform_.translation_, size_);
}

void MovingFloor::Draw(const ViewProjection& viewProjection)
{
	models_[0]->Draw(worldTransform_, viewProjection);
}

void MovingFloor::SetPosition(const Vector3& position)
{
	worldTransform_.translation_ = position;
	startPosition_ = position;
	aabb_.min = Subtract(worldTransform_.translation_, size_);
	aabb_.max = Add(worldTransform_.translation_, size_);
	worldTransform_.UpdateMatrix();
}
