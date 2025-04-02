#include "MoveFloor.h"

#include "GlobalVariables.h"
#include "EngineTimeStep.h"

void MoveFloor::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "MoveFloor";
	amplitude_ = globalVariables->GetFloatValue(groupName, "Amplitude");
	cycle_ = globalVariables->GetFloatValue(groupName, "Cycle");

	if (GetTag() == Tag::kMoveFloor_L) {
		amplitude_ *= -1.0f;
	}
	time_ = 0.0f;

	// 衝突マスク設定
	SetCollisionMask(kCollisionAttributePlayer);
}

void MoveFloor::Update()
{
	worldTransform_.translation_.x += amplitude_;

	time_ += yunity::fixedTimeStep_;
	if (cycle_ < time_) {
		amplitude_ *= -1.0f;
		time_ = 0.0f;
	}

	worldTransform_.UpdateMatrix();

}
