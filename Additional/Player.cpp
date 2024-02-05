#include "Player.h"
#include "Input.h"

void Player::Initialize()
{
	worldTransfrom_.Initialize();
	HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));

	Create(&worldTransfrom_, Type::kSphere);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);

}

void Player::Update()
{
	
	if (Input::GetInstance()->PushKey(DIK_A)) {
		//worldTransfrom_.translation_.x -= 0.1f;
		SetVelocity(Vector3{ -0.1f, 0.0f, 0.0f });
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		//worldTransfrom_.translation_.x += 0.1f;
		SetVelocity(Vector3{ 0.1f, 0.0f, 0.0f });
	}
	Step(RotationType::Euler);

	//worldTransfrom_.UpdateMatrix(RotationType::Euler);
}

void Player::Draw(const Camera& camera)
{
	HitBox_->Draw(worldTransfrom_);
}
