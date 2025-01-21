#include "RotationCube.h"

#include "GlobalVariables.h"

void RotationCube::Initialize(float direction)
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "RotationCude";
	torque_ = globalVariables->GetFloatValue(groupName, "Torque");

	torque_ *= direction;
	SetShape(Shape::kOBB);
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
