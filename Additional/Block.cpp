#include "Block.h"

void Block::Initialize()
{
	worldTransfrom_.Initialize();
	HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));

	Create(&worldTransfrom_, Type::kSphere);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
}

void Block::Update()
{
}

void Block::Draw(const Camera& camera)
{
	HitBox_->Draw(worldTransfrom_, camera);
}
