#include "ForceField.h"

#include "EngineTimeStep.h"
#include "ModelManager.h"

void ForceField::Initialize()
{
	time_ = 0.0f;
	isWind_ = false;
	/*windParticle_ = std::make_unique<WindParticle>();
	windParticle_->Initialize(camera_);*/
	model_ = std::make_unique<yunity::Model>();
	model_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"));
}

void ForceField::Update()
{
	time_ += yunity::fixedTimeStep_;
	if (time_ > limitTime_) {
		isWind_ ^= true;
		time_ = 0.0f;
	}
	if (isWind_) {
		//windParticle_->Spawn(worldTransform_.GetMatWorldTranslation());
	}
	//windParticle_->Update();
}

void ForceField::Draw()
{
	if (isWind_) {
		//windParticle_->Draw();
	}

}

void ForceField::OnTriggerEvent()
{
	if (isWind_) {
		if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeTrampoline) {
			GetHitBody()->AddForce({ 0.0f, 10.0f, 0.0f }, ForceMode::kImpulse);
		}
	}
}
