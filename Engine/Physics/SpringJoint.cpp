#include "SpringJoint.h"

void yunity::SpringJoint::CreateSpringJoint(Body* bodyA, Body* bodyB)
{
	bodyA_ = bodyA;
	bodyB_ = bodyB;

}

void yunity::SpringJoint::EnableSpring(int index, bool onOff)
{
	springEnabled_[index] = onOff;
}

void yunity::SpringJoint::SetStiffness(int index, float stiffness)
{
	stiffness_[index] = stiffness;
}

void yunity::SpringJoint::SetDamping(int index, float damping)
{
	dampingCoefficient_[index] = damping;
}

void yunity::SpringJoint::SetEquilibriumPoint()
{
	// 差分を取得
	Vector3 v = Subtract(bodyA_->GetTranslation(), bodyB_->GetTranslation());

	// 絶対値に変換
	v.x = std::abs(v.x);
	v.y = std::abs(v.y);
	v.z = std::abs(v.z);

	// 長さを取得
	float diff = Length(v);

	for (int i = 0; i < 3; i++) { // 平衡点を設定
		equilibriumPoint_[i] = diff;
	}

}

void yunity::SpringJoint::SetEquilibriumPoint(int index)
{
	// 差分を取得
	Vector3 v = Subtract(bodyA_->GetTranslation(), bodyB_->GetTranslation());

	// 絶対値に変換
	v.x = std::abs(v.x);
	v.y = std::abs(v.y);
	v.z = std::abs(v.z);

	// 長さを取得
	float diff = Length(v);

	// 平衡点を設定
	equilibriumPoint_[index] = diff;
}

void yunity::SpringJoint::SetEquilibriumPoint(int index, float value)
{
	equilibriumPoint_[index] = value;
}

void yunity::SpringJoint::Solve()
{
	Vector3 force1;
	Vector3 force2;
	for (int index = 0; index < 3; index++) {
		if (!springEnabled_[index]) { // 無効軸の場合
			continue;
		}
		// バネの運動
		force1 = Spring(bodyA_, bodyB_, equilibriumPoint_[index], stiffness_[index], dampingCoefficient_[index]);
		force2 = Spring(bodyB_, bodyA_, equilibriumPoint_[index], stiffness_[index], dampingCoefficient_[index]);
		// 力を加える
		if (index == 0) { // x軸
			bodyA_->AddForce(Vector3(force1.x, 0.0f, 0.0f), Body::ForceMode::kForce);
			bodyB_->AddForce(Vector3(force2.x, 0.0f, 0.0f), Body::ForceMode::kForce);
		}
		else if (index == 1) { // y軸
			bodyA_->AddForce(Vector3(0.0f, force1.y, 0.0f), Body::ForceMode::kForce);
			bodyB_->AddForce(Vector3(0.0f, force2.y, 0.0f), Body::ForceMode::kForce);
		}
		else { // z軸
			bodyA_->AddForce(Vector3(0.0f, 0.0f, force1.z), Body::ForceMode::kForce);
			bodyB_->AddForce(Vector3(0.0f, 0.0f, force2.z), Body::ForceMode::kForce);
		}

	}

}

Vector3 yunity::SpringJoint::Spring(Body* bodyA, Body* bodyB, float naturalLength, float stiffness, float dampingCoefficient)
{
	// 差分を取得
	Vector3 diff = Subtract(bodyA->GetMatWorldTranslation(), bodyB->GetMatWorldTranslation());
	// 長さを取得
	float length = Length(diff);
	if (length != 0.0f) { // 長さが0でない場合
		// アンカーを取得
		Vector3 anchor = bodyB->GetMatWorldTranslation();
		// 方向を取得
		Vector3 direction = Normalize(diff);
		// 自然な位置を計算
		Vector3 restPosition = Add(anchor, Multiply(naturalLength, direction));
		// 差分を計算
		Vector3 displacement = Multiply(length, Subtract(bodyA->GetMatWorldTranslation(), restPosition));
		// 復元力を計算
		Vector3 restoringForce = Multiply(-stiffness, displacement);
		// 減衰力を計算
		Vector3 dampingForce = Multiply(-dampingCoefficient, bodyA->GetVelocity());
		// 力を計算
		Vector3 force = Add(restoringForce, dampingForce);
		return force;
	}
	return { 0.0f, 0.0f, 0.0f };
}


