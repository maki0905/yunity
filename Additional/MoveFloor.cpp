#include "MoveFloor.h"


void MoveFloor::InitializeDirection(Tag type)
{
	if (type == Tag::kMoveFloor_L) {
		amplitude_ = -0.08f;
	}
	else if(type == Tag::kMoveFloor_R){
		amplitude_ = 0.08f;
	}
}

void MoveFloor::Update()
{
	worldTransform_.translation_.x += amplitude_;

	time_ += secound_;
	if (cycle_ < time_) {
		amplitude_ *= -1.0f;
		time_ = 0.0f;
	}

	worldTransform_.UpdateMatrix();

}
