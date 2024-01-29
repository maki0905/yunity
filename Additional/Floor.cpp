#include "Floor.h"


void Floor::Initialize(const std::vector<Model*>& models)
{
	BaseObject::Initialize(models);
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix(RotationType::Euler);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeFloor);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeFloor);
	size_ = { 15.0f, 0.25f, 15.0f };
}

void Floor::Update()
{


}

void Floor::Draw(const Camera& camera)
{
	models_[0]->Draw(worldTransform_, camera);
}

void Floor::SetPosition(const Vector3& position)
{
	worldTransform_.translation_ = position;
	aabb_.min = Subtract(worldTransform_.translation_, size_);
	aabb_.max = Add(worldTransform_.translation_, size_);
	worldTransform_.UpdateMatrix(RotationType::Euler);
}

void Floor::SetScale(const Vector3& scale)
{
	worldTransform_.scale_ = scale;
	size_ = Multiply(scale.x, size_);
	aabb_.min = Subtract(worldTransform_.translation_, size_);
	aabb_.max = Add(worldTransform_.translation_, size_);
	worldTransform_.UpdateMatrix(RotationType::Euler);
}
