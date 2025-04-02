#include "RotationCube.h"

#include "GlobalVariables.h"

void RotationCube::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "RotationCude";
	torque_ = globalVariables->GetFloatValue(groupName, "Torque");

	if (GetTag() == Tag::kCounterClockwise) {
		torque_ *= -1.0f;
	}

	SetShape(Shape::kOBB);

	// 衝突マスク設定
	SetCollisionMask((kCollisionAttributePlayer | kCollisionAttributeCoin));
}

void RotationCube::Update()
{
	worldTransform_.rotation_.z += torque_ * DegToRad();
	if (worldTransform_.rotation_.z >= 360.0f * DegToRad() || worldTransform_.rotation_.z <= -360.0f * DegToRad()) {
		worldTransform_.rotation_.z = 0.0f;
	}

#ifdef _DEBUG
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "RotationCude";
	torque_ /= std::abs(torque_);
	torque_ *= globalVariables->GetFloatValue(groupName, "Torque");
#endif // _DEBUG


}
