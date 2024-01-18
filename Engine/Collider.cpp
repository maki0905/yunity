#include "Collider.h"
#include <cmath>

void Collider::Create(WorldTransform* worldTransform, Type type)
{
	type_ = type;
	worldTransform_ = worldTransform;
	
	switch (type_)
	{
	case Collider::kSphere:
		sphere_.center = { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] };
		sphere_.radius = 1.0f;
		//sphere_. = { .center = {worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]}, .radius = {1.0f}};
		//sphere_ = sphere
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		break;
	case Collider::kCapsule:
		break;
	case Collider::kOBB:
		break;
	default:
		break;
	}
	//worldTransform_->UpdateMatrix(RotationType::Euler);

}

void Collider::Step(RotationType rotationType)
{
	if (fabs(velocity_.x) > 0.01f || fabs(velocity_.y) > 0.01f || fabs(velocity_.z) > 0.01f) {
		float magnitude = miu_ * (mass_ * gravityScale_);

		Vector3 direction = {};
		if (velocity_.x != 0) {
			direction.x = velocity_.x / std::fabs(velocity_.x);
		}
		if (velocity_.y != 0) {
			direction.y = velocity_.y / std::fabs(velocity_.y);
		}
		if (velocity_.z != 0) {
			direction.z = velocity_.z / std::fabs(velocity_.z);
		}
		direction = Multiply(-1.0f, direction);

		Vector3 frictionalForce = Multiply(magnitude, direction);
		acceleration_ = Multiply(1.0f / mass_, frictionalForce);

		if (std::fabs(acceleration_.x) > std::fabs(velocity_.x)) {
			acceleration_.x = velocity_.x;
		}
		if (std::fabs(acceleration_.z) > std::fabs(velocity_.z)) {
			acceleration_.z = velocity_.z;
		}

		velocity_ = Add(velocity_, acceleration_);

		worldTransform_->translation_ = Add(worldTransform_->translation_, velocity_);
		worldTransform_->UpdateMatrix(rotationType);
	}

	switch (type_)
	{
	case Collider::kSphere:
		sphere_.center = { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] };
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		break;
	case Collider::kCapsule:
		break;
	case Collider::kOBB:
		break;
	default:
		break;
	}


}

void Collider::OnCollision()
{
}

Vector3 Collider::GetWorldPosition()
{
	return Vector3();
}

void Collider::SetVelocity(const Vector3& velocity)
{
	velocity_ = Add(velocity_, velocity);
}
