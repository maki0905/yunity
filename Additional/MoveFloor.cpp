#include "MoveFloor.h"

void MoveFloor::Update()
{
	worldTransform_.translation_.x += amplitude_;

	time_ += 1.0f / 60.0f;
	if (cycle_ < time_) {
		amplitude_ *= -1.0f;
		time_ = 0.0f;
	}

	worldTransform_.UpdateMatrix();

}
