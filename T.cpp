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

void T::Draw()
{
	triangle_->Draw(worldTransform_);

}
