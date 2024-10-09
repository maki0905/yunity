#pragma once
#include  "Joint.h"
class PulleyJoint : public Joint {
public:
	void CreatePulleyJoint(Body* bodyA, Body* bodyB, Vector3 groundAnchorA, Vector3 groundAnchorB, Vector3 anchorA, Vector3 anchorB, float ratio);
	
	Vector3 GetGroundAnchorA() { return groundAnchorA_; }
	Vector3 GetGroundAnchorB() { return groundAnchorB_; }
	void SetGroundAnchor(Vector3 groundAnchorA, Vector3 groundAnchorB) { groundAnchorA_ = groundAnchorA; groundAnchorB_ = groundAnchorB; };
	void SetGroundAnchorA(Vector3 groundAnchorA) { groundAnchorA_ = groundAnchorA; }
	void SetGroundAnchorB(Vector3 groundAnchorB) { groundAnchorB_ = groundAnchorB; }
	
	Vector3 GetAnchorA() { return anchorA_; }
	Vector3 GetAnchorB() { return anchorB_; }
	void SetAnchor(Vector3 anchorA, Vector3 anchorB) { anchorA_ = anchorA; anchorB_ = anchorB; }
	void SetAnchorA(Vector3 anchorA) { anchorA_ = anchorA; }
	void SetAnchorB(Vector3 anchorB) { anchorB_ = anchorB; }
	
	float GetRatio() { return ratio_; }
	void SetRatio(float ratio) { ratio_ = ratio; }

	float GetLengthA() { return lengthA_; }
	float GetLengthB() { return lengthB_; }
	
	void Solve() override;

private:
	void SolveVelocity();
	void SolvePosition();

private:
	Vector3 groundAnchorA_;
	Vector3 groundAnchorB_;
	Vector3 anchorA_;
	Vector3 anchorB_;
	Vector3 directionA_;
	Vector3 directionB_;
	float lengthA_;
	float lengthB_;
	float constant_;
	float ratio_;
	float mass_;

};