#include "SpringJoint.h"

void SpringJoint::CreateSpringJoint(Body* bodyA, Body* bodyB)
{
	bodys_[0] = bodyA;
	bodys_[1] = bodyB;
}

void SpringJoint::enableSpring(int index, bool onOff)
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
	Vector3 diff;
	float length;
	Vector3 direction;
	Vector3 restPosition;
	Vector3 displacement;
	Vector3 restoringForce;
	Vector3 dampingForce;
	Vector3 force;
	for (int index = 0; index < 3; index++) {
		if (!springEnabled_[index]) {
			continue;
		}
		Vector3 diff = Subtract(bodys_[0]->GetMatWorldTranslation(), bodys_[1]->GetMatWorldTranslation());
		float length = Length(diff);
		if (length != 0.0f)
		{
			/*Vector3 anchor = 
			direction = Normalize(diff);
			restPosition = Add(anchor, Multiply(naturalLength, direction));
			displacement = Multiply(length, Subtract(position, restPosition));
			restoringForce = Multiply(-stiffness, displacement);
			dampingForce = Multiply(-dampingCoefficient, velocity_);
			force = Add(restoringForce, dampingForce);*/
			
		}
	}

}

