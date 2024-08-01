#include "MoveFloor.h"

void MoveFloor::InitializeDirection()
{
	if (GetCollisionAttribute() == kCollisionAttributeMoveFloorLeft) {
		amplitude_ = -0.08f;
	}
	else {
		amplitude_ = 0.08f;
	}
}

void MoveFloor::Update()
{
	worldTransform_.translation_.x += amplitude_;

	time_ += 2.0f / 60.0f;
	if (cycle_ < time_) {
		amplitude_ *= -1.0f;
		time_ = 0.0f;
	}

	worldTransform_.UpdateMatrix();

}
