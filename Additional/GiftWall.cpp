#include "GiftWall.h"

void GiftWall::Update()
{
	if (std::abs(worldTransform_.rotation_.y) < 160.0f * DegToRad() && std::abs(worldTransform_.rotation_.y) > 0.0f) {
		if (worldTransform_.rotation_.z < 90.0f * DegToRad()) {
			worldTransform_.rotation_.z -= rotateSpeed_ * DegToRad();
		}
	}
	else {
		if (worldTransform_.rotation_.x < 90.0f * DegToRad()) {
			worldTransform_.rotation_.x -= rotateSpeed_ * DegToRad();
		}
	}

}
