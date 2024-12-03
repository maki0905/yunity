#include "PillarTop.h"

void PillarTop::Initialize()
{
	isRoll_ = false;
}

void PillarTop::Update()
{
	Vector3 rotation = GetWorldTransform()->rotation_;
	rotation.z -= toruqe_ * DegToRad();
	if (rotation.z <= -180.0f * DegToRad()) {
		rotation.z = 0.0f;
	}
	SetRotation(rotation);
}
