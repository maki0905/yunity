#include "Coin.h"

//void Coin::Initialize()
//{
//	isActive_ = true;
//	SetShape(Shape::kSphere);
//	SetSize({ 1.0f, 1.0f, 1.0f });
//}

void Coin::OnCollisionEvent(Body* body)
{
	if (body->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = false;
	}

}

void Coin::OnTriggerEvent(Body* body)
{
	if (body->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = false;
	}
}
