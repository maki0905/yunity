#include "Collider.h"
#include <cmath>
#include "ImGuiManager.h"
#include <algorithm>

void yunity::Collider::CreateCollider(WorldTransform* worldTransform, Shape shape, Camera* camera)
{
	shape_ = shape;
	worldTransform_ = worldTransform;

	switch (shape_)
	{
	case Collider::kSphere:
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kSphere));
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
		break;
	case Collider::kCapsule:
		break;
	case Collider::kOBB:
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
		break;
	default:
		break;
	}

	HitBox_->SetCamera(camera);
	worldTransform_HitBox_.Initialize();
}

void yunity::Collider::CreateCollider(WorldTransform* worldTransform, Shape shape, Camera* camera, const Vector3& size)
{
	shape_ = shape;
	worldTransform_ = worldTransform;
	size_ = size;

	switch (shape_)
	{
	case Collider::kSphere:
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kSphere));
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
		break;
	case Collider::kCapsule:
		break;
	case Collider::kOBB:
		break;
	default:
		break;
	}

	HitBox_->SetCamera(camera);
	worldTransform_HitBox_.Initialize();
	worldTransform_HitBox_.scale_ = size;

}

void yunity::Collider::HitBox()
{
	if (shape_ == kSphere) {
		worldTransform_HitBox_.scale_ = Multiply(size_.x, worldTransform_->scale_);
	}
	else {
		worldTransform_HitBox_.scale_ = Multiply(size_, worldTransform_->scale_);
	}
	worldTransform_HitBox_.rotation_ = worldTransform_->rotation_;
	worldTransform_HitBox_.translation_ = GetColliderCenter();
	worldTransform_HitBox_.UpdateMatrix();

	HitBox_->Draw(worldTransform_HitBox_);
}

void yunity::Collider::HitBox(Camera* camera)
{
	if (shape_ == kSphere) {
		worldTransform_HitBox_.scale_ = Multiply(size_.x, worldTransform_->scale_);
	}
	else {
		worldTransform_HitBox_.scale_ = Multiply(size_, worldTransform_->scale_);
	}
	worldTransform_HitBox_.rotation_ = worldTransform_->rotation_;
	worldTransform_HitBox_.translation_ = GetColliderCenter();
	worldTransform_HitBox_.UpdateMatrix();
	HitBox_->SetCamera(camera);

	HitBox_->Draw(worldTransform_HitBox_);
}

Vector3 yunity::Collider::GetHitBoxSize()
{
	Vector3 result = { 0.0f, 0.0f, 0.0f };
	switch (shape_)
	{
	case Collider::kSphere:
		result = Multiply(size_, worldTransform_->scale_);
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		result = Multiply(Multiply(0.5f, size_), worldTransform_->scale_);
		break;
	case Collider::kCapsule:
		break;
	case Collider::kOBB:
		result = Multiply(Multiply(0.5f, size_), worldTransform_->scale_);
		break;
	default:
		break;
	}

	return  result;
}

AABB yunity::Collider::GetAABB()
{
	return AABB(Subtract(worldTransform_->translation_, GetHitBoxSize()), Add(worldTransform_->translation_, GetHitBoxSize()));
}

