#include "FixedJoint.h"

void yunity::FixedJoint::CreateFixedJoint(Body* bodyA, Body* bodyB)
{
	bodyA_ = bodyA;
	bodyB_ = bodyB;

	initialPositionOffset = Subtract(bodyB->GetMatWorldTranslation(), bodyA->GetMatWorldTranslation());

}

void yunity::FixedJoint::Clear()
{
	bodyA_ = nullptr;
	bodyB_ = nullptr;
	initialPositionOffset.SetZero();
}

void yunity::FixedJoint::Solve()
{
	if (bodyA_ != nullptr && bodyB_ != nullptr) {
		Matrix4x4 rotate = MakeRotateXYZMatrix(bodyA_->GetWorldTransform()->rotation_);
		Vector3 offset = TransformNormal(initialPositionOffset, rotate);
		bodyB_->SetTranslation(Add(bodyA_->GetMatWorldTranslation(), offset));
	}
}
