#include "FixedJoint.h"

void FixedJoint::CreateFixedJoint(Body* bodyA, Body* bodyB)
{
	bodyA_ = bodyA;
	bodyB_ = bodyB;

	initialPositionOffset = Subtract(bodyB->GetMatWorldTranslation(), bodyA->GetMatWorldTranslation());

}

void FixedJoint::Clear()
{
	bodyA_ = nullptr;
	bodyB_ = nullptr;
	initialPositionOffset.SetZero();
}

void FixedJoint::Solve()
{
	if (bodyA_ != nullptr && bodyB_ != nullptr) {
		bodyB_->SetTranslation(Add(bodyA_->GetMatWorldTranslation(), initialPositionOffset));
	}
}
