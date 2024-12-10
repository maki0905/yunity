#include "RotationCube.h"


void RotationCube::Initialize(float torque)
{
	torque_ = torque;
	SetShape(Shape::kOBB);
}

void RotationCube::Update()
{
	worldTransform_.rotation_.z += torque_ * DegToRad();
	if (worldTransform_.rotation_.z >= 360.0f * DegToRad() || worldTransform_.rotation_.z <= -360.0f * DegToRad()) {
		worldTransform_.rotation_.z = 0.0f;
	}
}
