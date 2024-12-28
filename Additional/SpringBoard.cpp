#include "SpringBoard.h"



void SpringBoard::Initialize()
{
	anchor_ = std::make_unique<yunity::Object3D>();
	anchor_->Initialize(GetWorld(), Shape::kAABB);
	anchor_->SetCollisionAttribute(kCollisionAttributeFloor);
	//anchor_->SetIsTrigger(true);
	anchor_->SetPosition({worldTransform_.translation_.x, worldTransform_.translation_.y - 1.0f, worldTransform_.translation_.z });
	GetWorld()->Add(anchor_.get());
	SetSpringJoint();
	fixedPosition_ = worldTransform_.translation_;
}

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

void SpringBoard::SetSpringJoint()
{
	springJoint_ = std::make_unique<yunity::SpringJoint>();
	springJoint_->CreateSpringJoint(this, anchor_.get());
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
