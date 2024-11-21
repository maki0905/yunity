#include "PulleyJoint.h"

void yunity::PulleyJoint::CreatePulleyJoint(Body* bodyA, Body* bodyB, Vector3 groundAnchorA, Vector3 groundAnchorB, Vector3 anchorA, Vector3 anchorB, float ratio)
{
	bodyA_ = bodyA;
	bodyB_ = bodyB;
	groundAnchorA_ = groundAnchorA;
	groundAnchorB_ = groundAnchorB;
	anchorA_ = anchorA;
	anchorB_ = anchorB;
	ratio_ = ratio;

	directionA_ = Subtract(bodyA->GetMatWorldTranslation(), groundAnchorA);
	directionB_ = Subtract(bodyB->GetMatWorldTranslation(), groundAnchorB);

	lengthA_ = Length(directionA_);
	lengthB_ = Length(directionB_);

	directionA_.Normalize();
	directionB_.Normalize();

	constant_ = lengthA_ + ratio * lengthB_;

	mass_ = bodyA->GetInverseMass() + ratio * ratio * bodyB->GetInverseMass();
	if (mass_ > 0.0f) {
		mass_ = 1.0f / mass_;
	}

}

void yunity::PulleyJoint::Solve()
{
	SolveVelocity();
	SolvePosition();

}

void yunity::PulleyJoint::SolveVelocity()
{
	// 各ボディの速度取得
	Vector3 velocityA = bodyA_->GetVelocity();
	Vector3 velocityB = bodyB_->GetVelocity();

	// 速度エラーを計算
	float cdot = -Dot(directionA_, velocityA) - ratio_ * Dot(directionB_, velocityB);

	// インパルスを計算
	float impulse = -mass_ * cdot;

	// ボディにかかる力を計算
	Vector3 forceA = Multiply(-impulse, directionA_);
	Vector3 forceB = Multiply(-ratio_ * impulse, directionB_);

	// ボディに適用
	bodyA_->AddForce(forceA, Body::ForceMode::kImpulse);
	bodyB_->AddForce(forceB, Body::ForceMode::kImpulse);
}

void yunity::PulleyJoint::SolvePosition()
{
	Vector3 positionA = bodyA_->GetMatWorldTranslation();
	Vector3 positionB = bodyB_->GetMatWorldTranslation();

	// ボディと固定アンカーの間のベクトルを計算
	Vector3 directionA = Subtract(positionA, groundAnchorA_);
	Vector3 directionB = Subtract(positionB, groundAnchorB_);

	// 各ベクトルの長さを取得
	float lengthA = Length(directionA);
	float lengthB = Length(directionB);

	if (lengthA > 10.0f * 0.005f) {
		directionA.Normalize();
	}
	else {
		directionA.SetZero();
	}

	if (lengthB > 10.0f * 0.005f) {
		directionB.Normalize();
	}
	else {
		directionB.SetZero();
	}
	// 位置制約エラーを計算
	float c = constant_ - lengthA - ratio_ * lengthB;
	//float linearError = std::fabsf(c);

	// インパルスを計算
	float impulse = -mass_ * c;
	Vector3 forceA = Multiply(-impulse, directionA);
	Vector3 forceB = Multiply(-ratio_ * impulse, directionB);

	// 位置を調整
	positionA = Add(positionA, Multiply(1.0f / bodyA_->GetMass(), forceA));
	positionB = Add(positionB, Multiply(1.0f / bodyB_->GetMass(), forceB));
	bodyA_->SetTranslation(positionA);
	bodyB_->SetTranslation(positionB);

}
