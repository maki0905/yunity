#include "T.h"

void T::Initialize()
{
	worldTransform_.Initialize();
	triangle_ = Triangle::Create();

}

void T::Update()
{
	worldTransform_.rotation_.y += 0.01f;
	worldTransform_.UpdateMatrix();

}

void T::Draw(const ViewProjection& viewProjection)
{
	triangle_->Draw(worldTransform_, viewProjection, textureHandle_);

}
