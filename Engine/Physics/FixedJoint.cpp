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
		// 回転行列を作成
		Matrix4x4 rotate = MakeRotateXYZMatrix(bodyA_->GetWorldTransform()->rotation_);
		// 初期位置を回転
		Vector3 offset = TransformNormal(initialPositionOffset, rotate);
		// 位置を設定
		bodyB_->SetTranslation(Add(bodyA_->GetMatWorldTranslation(), offset));
	}
}
