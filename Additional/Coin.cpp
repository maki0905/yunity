#include "Coin.h"

void Coin::Initialize()
{
	model_->SetBlendModeType(yunity::kNormal);
	particle_ = std::make_unique<FloatingParticle>();
	particle_->Initialize(camera_);
	particle_->Spawn(worldTransform_.translation_);

	// 衝突マスク設定
	SetCollisionMask((kCollisionAttributePlayer | kCollisionAttributeFloor | kCollisionAttributeMove));
}

void Coin::Update()
{
	if (!isActive_) {
		GetWorld()->Take(this);
		particle_->SetTarget(player_->GetMatWorldTranslation());
	}
	if (particle_->GetIsActive()) {
		particle_->Update();
	}

}

void Coin::Draw()
{
	if (isActive_) {
		model_->Draw(worldTransform_);
	}
	if (particle_->GetIsActive()) {
		particle_->Draw();
	}
}

void Coin::OnCollisionEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = false;
		player_ = GetHitBody();
		SetIsTrigger(true);
	}
}

void Coin::OnTriggerEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = false;
		player_ = GetHitBody();
		SetIsTrigger(true);
	}
}

