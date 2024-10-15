#include "SpringJoint.h"

void SpringJoint::CreateSpringJoint(Body* bodyA, Body* bodyB)
{
	bodyA_ = bodyA;
	bodyB_ = bodyB;

}

void SpringJoint::EnableSpring(int index, bool onOff)
{
	springEnabled_[index] = onOff;
}

void SpringJoint::SetStiffness(int index, float stiffness)
{
	stiffness_[index] = stiffness;
}

void SpringJoint::SetDamping(int index, float damping)
{
	dampingCoefficient_[index] = damping;
}

void SpringJoint::SetEquilibriumPoint()
{
	Vector3 v = Subtract(bodyA_->GetTranslation(), bodyB_->GetTranslation());
	v.x = std::abs(v.x);
	v.y = std::abs(v.y);
	v.z = std::abs(v.z);

	float diff = Length(v);

	for (int i = 0; i < 3; i++)
	{
		equilibriumPoint_[i] = diff;
	}

}

void SpringJoint::SetEquilibriumPoint(int index)
{
	Vector3 v = Subtract(bodyA_->GetTranslation(), bodyB_->GetTranslation());
	v.x = std::abs(v.x);
	v.y = std::abs(v.y);
	v.z = std::abs(v.z);
	float diff = Length(v);
	equilibriumPoint_[index] = diff;
}

void SpringJoint::SetEquilibriumPoint(int index, float value)
{
	equilibriumPoint_[index] = value;
}

void SpringJoint::Solve()
{
	Vector3 force;
	for (int index = 0; index < 3; index++) {
		if (!springEnabled_[index]) {
			continue;
		}
		force = Spring(bodyA_, bodyB_, equilibriumPoint_[index], stiffness_[index], dampingCoefficient_[index]);
		bodyA_->AddForce(force, 0);
		force = Spring(bodyB_, bodyA_, equilibriumPoint_[index], stiffness_[index], dampingCoefficient_[index]);
		bodyB_->AddForce(force, 0);

	}

}

Vector3 SpringJoint::Spring(Body* bodyA, Body* bodyB, float naturalLength, float stiffness, float dampingCoefficient)
{
	Vector3 diff = Subtract(bodyA->GetMatWorldTranslation(), bodyB->GetMatWorldTranslation());
	float length = Length(diff);
	if (length != 0.0f) {
		Vector3 anchor = bodyB->GetMatWorldTranslation();
		Vector3 direction = Normalize(diff);
		Vector3 restPosition = Add(anchor, Multiply(naturalLength, direction));
		Vector3 displacement = Multiply(length, Subtract(bodyA->GetMatWorldTranslation(), restPosition));
		Vector3 restoringForce = Multiply(-stiffness, displacement);
		Vector3 dampingForce = Multiply(-dampingCoefficient, bodyA->GetVelocity());
		Vector3 force = Add(restoringForce, dampingForce);
		return force;
	}
	return { 0.0f, 0.0f, 0.0f };
}


