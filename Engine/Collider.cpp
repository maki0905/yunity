#include "Collider.h"
#include <cmath>
#include "ImGuiManager.h"
#include <algorithm>

void Collider::Create(WorldTransform* worldTransform, Type type, RotationType rotationType, Camera* camera)
{
	type_ = type;
	rotationType_ = rotationType;
	worldTransform_ = worldTransform;
	
	switch (type_)
	{
	case Collider::kSphere:
		sphere_ = std::make_unique<Sphere>();
		sphere_->center = { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] };
		sphere_->radius = 1.0f;
		//sphere_. = { .center = {worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]}, .radius = {1.0f}};
		//sphere_ = sphere
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		aabb_ = std::make_unique<AABB>();
		aabb_->min = Subtract(worldTransform_->translation_, Multiply(1.25f, worldTransform_->scale_));
		aabb_->max = Add(worldTransform_->translation_, Multiply(1.25f, worldTransform_->scale_));
		//aabb_->min = 
		break;
	case Collider::kCapsule:
		break;
	case Collider::kOBB:
		break;
	default:
		break;
	}
	//worldTransform_->UpdateMatrix(RotationType::Euler);

	HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
	HitBox_->SetCamera(camera);
	worldTransform_HitBox_.Initialize();
	

}

void Collider::Solve()
{
	/*if (fabs(velocity_.x) > 0.01f || fabs(velocity_.y) > 0.01f || fabs(velocity_.z) > 0.01f) {
		float magnitude = miu_ * (mass_ * gravityScale_);

		Vector3 direction = {};
		if (velocity_.x != 0) {
			direction.x = velocity_.x / std::fabs(velocity_.x);
		}
		if (velocity_.y != 0) {
			direction.y = std::fabs(velocity_.y) / std::fabs(velocity_.y);
		}
		if (velocity_.z != 0) {
			direction.z = velocity_.z / std::fabs(velocity_.z);
		}
		direction = Multiply(-1.0f, direction);

		Vector3 frictionalForce = Multiply(magnitude, direction);
		acceleration_ = Multiply(1.0f / mass_, frictionalForce);
		acceleration_.y = -9.8f;

		if (std::fabs(acceleration_.x) > std::fabs(velocity_.x)) {
			acceleration_.x = velocity_.x;
		}
		if (std::fabs(acceleration_.z) > std::fabs(velocity_.z)) {
			acceleration_.z = velocity_.z;
		}
		


		velocity_ = Add(velocity_, Multiply(1.0f / 60.0f, acceleration_));

		if (worldTransform_->translation_.y < 0.0f) {
			worldTransform_->translation_.y = 0.0f;
			velocity_.y = 0.0f;
		}

		worldTransform_->translation_ = Add(worldTransform_->translation_, Multiply(1.0f/ 60.0f, velocity_));
		
	}*/

	if (mass_ != 0.0f) {
		float magnitude = miu_ * (mass_ * gravityScale_);

		Vector3 direction = {};
		if (velocity_.x != 0) {
			direction.x = velocity_.x / std::fabs(velocity_.x);
		}
		if (velocity_.y != 0) {
			direction.y = std::fabs(velocity_.y) / std::fabs(velocity_.y);
		}
		if (velocity_.z != 0) {
			direction.z = velocity_.z / std::fabs(velocity_.z);
		}
		direction = Multiply(-1.0f, direction);

		Vector3 frictionalForce = Multiply(magnitude, direction);
		acceleration_ = Multiply(1.0f / mass_, frictionalForce);
		acceleration_.y = -9.8f;

		if (std::fabs(acceleration_.x) > std::fabs(velocity_.x)) {
			acceleration_.x = velocity_.x;
		}
		if (std::fabs(acceleration_.z) > std::fabs(velocity_.z)) {
			acceleration_.z = velocity_.z;
		}



		velocity_ = Add(velocity_, Multiply(1.0f / 60.0f, acceleration_));

		/*if (worldTransform_->translation_.y < 0.0f) {
			worldTransform_->translation_.y = 0.0f;
			velocity_.y = 0.0f;
		}*/

		worldTransform_->translation_ = Add(worldTransform_->translation_, Multiply(1.0f / 60.0f, velocity_));
	}
	
	worldTransform_->UpdateMatrix(rotationType_);

	switch (type_)
	{
	case Collider::kSphere:
		sphere_->center = { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] };
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		aabb_->min = Subtract(Vector3(worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]), Multiply(1.25f, worldTransform_->scale_));
		aabb_->max = Add(Vector3(worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]), Multiply(1.25f, worldTransform_->scale_));
		ImGui::Begin("AABB");
		ImGui::SliderFloat3("min", &aabb_->min.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("max", &aabb_->max.x, -100.0f, 100.0f);
		ImGui::End();
		break;
	case Collider::kCapsule:
		break;
	case Collider::kOBB:
		break;
	default:
		break;
	}


}

void Collider::HitBox()
{
	worldTransform_HitBox_.scale_ = Multiply(2.5f, worldTransform_->scale_);
	worldTransform_HitBox_.rotation_ = worldTransform_->rotation_;
	worldTransform_HitBox_.translation_ = worldTransform_->translation_;
	worldTransform_HitBox_.UpdateMatrix(rotationType_);

	HitBox_->Draw(worldTransform_HitBox_);
}

void Collider::OnCollision()
{

}

void Collider::resolveCollision(AABB& other)
{
	// 各軸ごとに押し戻すベクトルを計算
	float pushX = 0.0f, pushY = 0.0f, pushZ = 0.0f;

	// X軸方向の押し戻し
	if (aabb_->max.x > other.min.x && aabb_->min.x < other.min.x)
		pushX = other.min.x - aabb_->max.x;
	else if (aabb_->min.x < other.max.x && aabb_->max.x > other.max.x)
		pushX = other.max.x - aabb_->min.x;

	// Y軸方向の押し戻し
	if (aabb_->max.y > other.min.y && aabb_->min.y < other.min.y)
		pushY = other.min.y - aabb_->max.y;
	else if (aabb_->min.y < other.max.y && aabb_->max.y > other.max.y)
		pushY = other.max.y - aabb_->min.y;

	// Z軸方向の押し戻し
	if (aabb_->max.z > other.min.z && aabb_->min.z < other.min.z)
		pushZ = other.min.z - aabb_->max.z;
	else if (aabb_->min.z < other.max.z && aabb_->max.z > other.max.z)
		pushZ = other.max.z - aabb_->min.z;

	// 押し戻しベクトルを使用して位置を調整

	if (pushY != 0.0f) {
		worldTransform_->translation_.y += pushY;
		if (pushY > 0) {
			if (velocity_.y < 0) {
				float re = velocity_.y * -1.0f;
				velocity_.y += re;
			}
			//velocity_.y += 9.8f / 60.0f;

		}
		else {
			velocity_.y = 0.0f;
		}
		
	}
	else {
		worldTransform_->translation_.x += pushX;
		//worldTransform_->translation_.y += pushY;
		worldTransform_->translation_.z += pushZ;
	}
	

	/*min.x += pushX;
	min.y += pushY;
	min.z += pushZ;
	max.x += pushX;
	max.y += pushY;
	max.z += pushZ;*/
}



Vector3 Collider::GetWorldPosition()
{
	return Vector3();
}

void Collider::SetVelocity(const Vector3& velocity)
{
	velocity_ = Add(velocity_, velocity);
}
