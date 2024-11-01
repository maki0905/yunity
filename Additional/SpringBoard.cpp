#include "SpringBoard.h"



void SpringBoard::Update()
{
	worldTransform_.translation_.x = fixedPosition_.x;
	worldTransform_.translation_.z = fixedPosition_.z;
	if (flag_) {
		time_ += 1.0f / 120.0f;
		time_ = std::clamp(time_, 0.0f, 1.0f);
		springJoint_->SetEquilibriumPoint(1, Lerp({ 0.0f, equilibriumPoint_, 0.0f }, { 0.0f, 0.0f, 0.0f }, time_).y);
	}

}

void SpringBoard::SetSpringJoint(Object3D* obj)
{
	springJoint_ = std::make_unique<SpringJoint>();
	springJoint_->CreateSpringJoint(this, obj);
	springJoint_->SetDamping(1, damping_);
	springJoint_->SetStiffness(1, 20.0f);
	springJoint_->SetEquilibriumPoint(1, equilibriumPoint_);
	springJoint_->EnableSpring(1, false);
	GetWorld()->AddJoint(springJoint_.get());
	flag_ = false;
}

void SpringBoard::OnCollisionEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		if (!flag_) {
			springJoint_->EnableSpring(1, true);
			flag_ = true;
		}
		else {
			springJoint_->SetEquilibriumPoint(1, equilibriumPoint_);
			time_ = 0.0f;
		}
	}
}
