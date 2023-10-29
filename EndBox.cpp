#include "EndBox.h"

void EndBox::Initialize(Model* model)
{
	models_ = {model};
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 100.0f, 0.75f, 100.0f };
	worldTransform_.UpdateMatrix();

	size_ = { 1.5f, 0.75f, 1.5f };

	aabb_.min = Subtract(worldTransform_.translation_, size_);
	aabb_.max = Add(worldTransform_.translation_, size_);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeGoal);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeGoal);
}

void EndBox::Update()
{
}

void EndBox::Draw(const ViewProjection& viewProjection)
{
	models_[0]->Draw(worldTransform_, viewProjection);
}
