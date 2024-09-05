#pragma once

#include "Body.h"

class SpringJoint {
public:
	void CreateSpringJoint(Body* bodyA, Body* bodyB);
	void EnableSpring(int index, bool onOff);
	void SetStiffness(int index, float stiffness);
	void SetDamping(int index, float damping);
	void SetEquilibriumPoint();
	void SetEquilibriumPoint(int index);
	void SetEquilibriumPoint(int index, float value);
	void Solve();
	Vector3 Spring(Body* bodyA, Body* bodyB, float naturalLength, float stiffness, float dampingCoefficient);
private:
	Body* bodys_[2];
	bool springEnabled_[3];
	float equilibriumPoint_[3];
	float stiffness_[3];
	float dampingCoefficient_[3];

};