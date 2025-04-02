#include "TV.h"

#include "TextureManager.h"
#include "EngineTimeStep.h"
#include "GlobalVariables.h"

void TV::Initialize()
{
	if (GetCollisionAttribute() == kCollisionAttributeTV1) {
		textureHandle_[0] = yunity::TextureManager::Load("Models/TV/TVOffReticle.png");
		textureHandle_[1] = yunity::TextureManager::Load("Models/TV/TVOnReticle.png");
	}
	else if (GetCollisionAttribute() == kCollisionAttributeTV2) {
		textureHandle_[0] = yunity::TextureManager::Load("Models/TV/TVRButton.png");
		textureHandle_[1] = yunity::TextureManager::Load("Models/TV/TVRButtonPush.png");
	}

	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "TV";
	changeTime_ = globalVariables->GetFloatValue(groupName, "ChangeTime");

	SetCollisionMask(kCollisionAttributePlayer);
}

void TV::Update()
{
	time_ += yunity::fixedTimeStep_;
	if (time_ > changeTime_) {
		change_ ^= true;
		time_ = 0.0f;
	}
	isActive_ = false;
}

void TV::Draw()
{
	if (isActive_) {
		if (change_) {
			model_->Draw(worldTransform_, textureHandle_[0]);
		}
		else {
			model_->Draw(worldTransform_,textureHandle_[1]);
		}
	}

#ifdef _DEBUG
	Collider::HitBox(camera_);
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "TV";
	changeTime_ = globalVariables->GetFloatValue(groupName, "ChangeTime");
#endif // _DEBUG


}

void TV::OnTriggerEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = true;
	}
}

