#include "Block.h"

#include "ModelManager.h"
void Block::Initialize(Camera* camera)
{
	worldTransfrom_.Initialize();

	model_.reset(ModelManager::GetInstance()->CreateModel(obj, "startBox"));
	model_->SetCamera(camera);
	

	//CreateCollider(&worldTransfrom_, Type::kAABB, RotationType::Euler, camera);

	// 衝突属性を設定
	//SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	//SetCollisionMask(~kCollisionAttributeEnemy);
}

void Block::Update()
{
	worldTransfrom_.UpdateMatrix();

}

void Block::Draw()
{
	model_->Draw(worldTransfrom_, TextureManager::GetInstance()->Load("white1x1.png"));
	Collider::HitBox();
}
