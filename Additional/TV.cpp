#include "TV.h"

#include "TextureManager.h"

void TV::InitializeTexture()
{
	if (GetCollisionAttribute() == kCollisionAttributeTV1) {
		textureHandle_[0] = yunity::TextureManager::Load("Models/TV/TVABotton.png");
		textureHandle_[1] = yunity::TextureManager::Load("Models/TV/TVABottonPush.png");
	}
	else if (GetCollisionAttribute() == kCollisionAttributeTV2) {
		textureHandle_[0] = yunity::TextureManager::Load("Models/TV/TVRStickLeft.png");
		textureHandle_[1] = yunity::TextureManager::Load("Models/TV/TVRStickRight.png");
	}
	else {
		textureHandle_[0] = yunity::TextureManager::Load("Models/TV/TVRBotton.png");
		textureHandle_[1] = yunity::TextureManager::Load("Models/TV/TVRBottonPush.png");
	}

}

void TV::Update()
{
	

	time_ += 1.0f;
	if (time_ > 30.0f) {
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
#endif // _DEBUG


}

void TV::OnTriggerEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = true;
	}
}

