#pragma once
#include  "Joint.h"

/*
* @brief プーリージョイント
*/
class PulleyJoint : public Joint {
public:
	/// <summary>
	/// プーリージョイント生成
	/// </summary>
	/// <param name="bodyA"></param>
	/// <param name="bodyB"></param>
	/// <param name="groundAnchorA"></param>
	/// <param name="groundAnchorB"></param>
	/// <param name="anchorA"></param>
	/// <param name="anchorB"></param>
	/// <param name="ratio"></param>
	void CreatePulleyJoint(Body* bodyA, Body* bodyB, Vector3 groundAnchorA, Vector3 groundAnchorB, Vector3 anchorA, Vector3 anchorB, float ratio);

	/// <summary>
	/// getter
	/// </summary>
	/// <returns></returns>
	
	Vector3 GetGroundAnchorA() { return groundAnchorA_; }
	Vector3 GetGroundAnchorB() { return groundAnchorB_; }
	Vector3 GetAnchorA() { return anchorA_; }
	Vector3 GetAnchorB() { return anchorB_; }
	float GetRatio() { return ratio_; }
	float GetLengthA() { return lengthA_; }
	float GetLengthB() { return lengthB_; }

	/// <summary>
	/// setter
	/// </summary>
	/// <param name="groundAnchorA"></param>
	/// <param name="groundAnchorB"></param>
	
	void SetGroundAnchor(Vector3 groundAnchorA, Vector3 groundAnchorB) { groundAnchorA_ = groundAnchorA; groundAnchorB_ = groundAnchorB; };
	void SetGroundAnchorA(Vector3 groundAnchorA) { groundAnchorA_ = groundAnchorA; }
	void SetGroundAnchorB(Vector3 groundAnchorB) { groundAnchorB_ = groundAnchorB; }
	void SetAnchor(Vector3 anchorA, Vector3 anchorB) { anchorA_ = anchorA; anchorB_ = anchorB; }
	void SetAnchorA(Vector3 anchorA) { anchorA_ = anchorA; }
	void SetAnchorB(Vector3 anchorB) { anchorB_ = anchorB; }
	void SetRatio(float ratio) { ratio_ = ratio; }

	
	/// <summary>
	/// 解決
	/// </summary>
	void Solve() override;

private:
	/// <summary>
	/// 速度解決
	/// </summary>
	void SolveVelocity();

	/// <summary>
	/// 位置解決
	/// </summary>
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