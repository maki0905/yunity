#include "Pillar.h"

#include "ModelManager.h"
void Pillar::Initialize()
{
	top_ = std::make_unique<PillarTop>();
	top_->Object3D::Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Wood"), GetWorld(), yunity::Collider::kOBB);
	Vector3 pos = worldTransform_.translation_;
	pos.y += 8.0f;
	top_->SetTranslation(pos);
	top_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	top_->SetCamera(camera_);
	top_->SetCollisionAttribute(kCollisionAttributePillar);
	GetWorld()->Add(top_.get());
}

void Pillar::Update()
{
	top_->Update();
}

void Pillar::Draw()
{
	model_->Draw(worldTransform_);
	top_->Draw();
}
