#include "Coin.h"

void Coin::Initialize()
{
	isActive_ = true;
}

void Coin::OnCollisionEvent(Body* body)
{
	if (body->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = false;
	}

}
