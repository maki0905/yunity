#include "ForceField.h"

#include "EngineTimeStep.h"
#include "ModelManager.h"
#include "GlobalVariables.h"

void ForceField::Initialize()
{
	time_ = 0.0f;
	isWind_ = false;
	windParticle_ = std::make_unique<WindParticle>();
	windParticle_->Initialize(camera_);
	model_ = std::make_unique<yunity::Model>();
	model_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"));

	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "ForceField";
	limitTime_ = globalVariables->GetFloatValue(groupName, "LimitTime");
	force_ = globalVariables->GetFloatValue(groupName, "Force");

	// 衝突マスク設定
	SetCollisionMask((kCollisionAttributePlayer | kCollisionAttributeMove));
}

void ForceField::Update()
{
	time_ += yunity::fixedTimeStep_;
	if (time_ > limitTime_) {
		isWind_ ^= true;
		time_ = 0.0f;
	}
	if (isWind_) {
		if (int(time_ / yunity::fixedTimeStep_) % 2 == 0) {
			Vector3 spawnPos = worldTransform_.GetMatWorldTranslation();
			spawnPos.y = 0.0f;
			windParticle_->Spawn(spawnPos);
		}
	}
	else {
		windParticle_->Clear();
	}
	windParticle_->Update();
#ifdef _DEBUG
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "ForceField";
	limitTime_ = globalVariables->GetFloatValue(groupName, "LimitTime");
	force_ = globalVariables->GetFloatValue(groupName, "Force");
#endif // _DEBUG
}

void ForceField::Draw()
{
	if (isWind_) {
		windParticle_->Draw();
	}

}

void ForceField::OnTriggerEvent()
{
	if (isWind_) {
		if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeMove) {
			GetHitBody()->AddForce({ 0.0f, force_, 0.0f }, ForceMode::kImpulse);
		}
	}
}
