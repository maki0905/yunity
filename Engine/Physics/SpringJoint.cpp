#include "SpringJoint.h"

void SpringJoint::CreateSpringJoint(Body* bodyA, Body* bodyB)
{
	bodys_[0] = bodyA;
	bodys_[1] = bodyB;
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
	Vector3 v = Subtract(bodys_[0]->GetTranslation(), bodys_[1]->GetTranslation());
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
	Vector3 v = Subtract(bodys_[0]->GetTranslation(), bodys_[1]->GetTranslation());
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
		force = Spring(bodys_[0], bodys_[1], equilibriumPoint_[index], stiffness_[index], dampingCoefficient_[index]);
		bodys_[0]->AddForce(force, 0);
		force = Spring(bodys_[1], bodys_[0], equilibriumPoint_[index], stiffness_[index], dampingCoefficient_[index]);
		bodys_[1]->AddForce(force, 0);

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


