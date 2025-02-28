#include "SpringBoard.h"

#include "ModelManager.h"
#include "EngineTimeStep.h"
#include "GlobalVariables.h"

void SpringBoard::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "SpringBoard";
	stiffness_ = globalVariables->GetFloatValue(groupName, "Stiffness");
	equilibriumPoint_ = globalVariables->GetFloatValue(groupName, "EquilibriumPoint");
	damping_ = globalVariables->GetFloatValue(groupName, "Damping");
	anchorPosition_ = globalVariables->GetVector3Value(groupName, "AnchorPosition");
	elongationTime_ = globalVariables->GetFloatValue(groupName, "ElongationTime");

	anchor_ = std::make_unique<yunity::Object3D>();
	anchor_->Initialize(GetWorld(), yunity::ShapeType::kBox);
	anchor_->SetCollisionAttribute(kCollisionAttributeFloor);
	anchor_->SetPosition(Add(worldTransform_.translation_, anchorPosition_));
	GetWorld()->AddObject(anchor_.get());
	SetSpringJoint();
	fixedPosition_ = worldTransform_.translation_;

	springModel_ = std::make_unique<yunity::Model>();
	springModel_.reset((yunity::ModelManager::GetInstance()->CreateModel(obj, "Spring")));
	springModel_->SetCamera(camera_);
	springWorldTransform_.Initialize();
	springWorldTransform_.translation_ = worldTransform_.translation_;
}

void SpringBoard::Update()
{
	worldTransform_.translation_.x = fixedPosition_.x;
	worldTransform_.translation_.z = fixedPosition_.z;
	if (flag_) {
		time_ += yunity::fixedTimeStep_;
		time_ = std::clamp(time_, 0.0f, elongationTime_);
		springJoint_->SetEquilibriumPoint(1, Lerp({ 0.0f, equilibriumPoint_, 0.0f }, { 0.0f, 0.0f, 0.0f }, time_ / elongationTime_).y);
	}
	float scale = worldTransform_.translation_.y - fixedPosition_.y;
	springWorldTransform_.scale_.y = 1.0f + scale;
	springWorldTransform_.UpdateMatrix();

#ifdef _DEBUG
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "SpringBoard";
	stiffness_ = globalVariables->GetFloatValue(groupName, "Stiffness");
	equilibriumPoint_ = globalVariables->GetFloatValue(groupName, "EquilibriumPoint");
	damping_ = globalVariables->GetFloatValue(groupName, "Damping");
	anchorPosition_ = globalVariables->GetVector3Value(groupName, "AnchorPosition");
	elongationTime_ = globalVariables->GetFloatValue(groupName, "ElongationTime");
#endif // _DEBUG


}

void SpringBoard::Draw()
{
	model_->Draw(worldTransform_);
	springModel_->Draw(springWorldTransform_);
}

void SpringBoard::SetSpringJoint()
{
	springJoint_ = std::make_unique<yunity::SpringJoint>();
	springJoint_->CreateSpringJoint(this, anchor_.get());
	springJoint_->SetDamping(1, damping_);
	springJoint_->SetStiffness(1, stiffness_);
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
