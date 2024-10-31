#include "TV.h"

#include "TextureManager.h"

void TV::InitializeTexture()
{
	modelWorldTransform_ = worldTransform_;
	isGrow_ = false;
	if (GetCollisionAttribute() == kCollisionAttributeTV1) {
		textureHandle_[0] = TextureManager::Load("Models/TV/TVABotton.png");
		textureHandle_[1] = TextureManager::Load("Models/TV/TVABottonPush.png");
	}
	else if (GetCollisionAttribute() == kCollisionAttributeTV2) {
		textureHandle_[0] = TextureManager::Load("Models/TV/TVRStickLeft.png");
		textureHandle_[1] = TextureManager::Load("Models/TV/TVRStickRight.png");
	}
	else {
		textureHandle_[0] = TextureManager::Load("Models/TV/TVRBotton.png");
		textureHandle_[1] = TextureManager::Load("Models/TV/TVRBottonPush.png");
	}

}

void TV::Update()
{
	if (!preIsActive_ && isActive_ && !isGrow_) {
		isGrow_ = true;
		growTime_ = 0.0f;
	}

	if (isGrow_) {
		growTime_ += second_;
		growTime_ = std::clamp(growTime_, 0.0f, 1.0f);
		modelWorldTransform_.scale_ = Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, growTime_);
		if (growTime_ == 1.0f) {
			isGrow_ = false;
		}
	}
	else {
		growTime_ -= second_;
		growTime_ = std::clamp(growTime_, 0.0f, 1.0f);
		modelWorldTransform_.scale_ = Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, growTime_);
	}

	time_ += 1.0f;
	if (time_ > 30.0f) {
		change_ ^= true;
		time_ = 0.0f;
	}
	preIsActive_ = isActive_;
	isActive_ = false;

	modelWorldTransform_.UpdateMatrix();
}

void TV::Draw()
{
	if (isActive_) {
		if (change_) {
			model_->Draw(modelWorldTransform_, textureHandle_[0]);
		}
		else {
			model_->Draw(modelWorldTransform_,textureHandle_[1]);
		}
	}

#ifdef _DEBUG
	Collider::HitBox(camera_);
#endif // _DEBUG


}

void TV::OnTriggerEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = true;
	}
}

