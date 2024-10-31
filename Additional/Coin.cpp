#include "Coin.h"

//void Coin::Initialize()
//{
//	isActive_ = true;
//	SetShape(Shape::kSphere);
//	SetSize({ 1.0f, 1.0f, 1.0f });
//}

void Coin::Update()
{
	

}

void Coin::Draw()
{
	if (isActive_) {
		model_->Draw(worldTransform_);
	}
}

void Coin::OnCollisionEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = false;
		SetIsTrigger(true);
	}
}

void Coin::OnTriggerEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = false;
		SetIsTrigger(true);
	}
}

