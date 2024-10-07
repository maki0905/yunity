#include "Collider.h"
#include <cmath>
#include "ImGuiManager.h"
#include <algorithm>

void Collider::CreateCollider(WorldTransform* worldTransform, Shape shape, Camera* camera)
{
	shape_ = shape;
	worldTransform_ = worldTransform;

	switch (shape_)
	{
	case Collider::kSphere:
		/*sphere_ = std::make_unique<Sphere>();
		sphere_->center = { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] };
		sphere_->radius = 1.0f;*/
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kSphere));
		//sphere_. = { .center = {worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]}, .radius = {1.0f}};
		//sphere_ = sphere
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		/*aabb_ = std::make_unique<AABB>();
		aabb_->min = Subtract(worldTransform_->translation_, Multiply(size_, worldTransform_->scale_));
		aabb_->max = Add(worldTransform_->translation_, Multiply(size_, worldTransform_->scale_));*/
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
		//aabb_->min = 
		break;
	case Collider::kCapsule:
		break;
	case Collider::kOBB:
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
		break;
	default:
		break;
	}
	//worldTransform_->UpdateMatrix(RotationType::Euler);

	HitBox_->SetCamera(camera);
	worldTransform_HitBox_.Initialize();
}

void Collider::CreateCollider(WorldTransform* worldTransform, Shape shape, Camera* camera, const Vector3& size)
{
	shape_ = shape;
	worldTransform_ = worldTransform;
	size_ = size;
	
	switch (shape_)
	{
	case Collider::kSphere:
		/*sphere_ = std::make_unique<Sphere>();
		sphere_->center = { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] };
		sphere_->radius = 1.0f;*/
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kSphere));
		//sphere_. = { .center = {worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]}, .radius = {1.0f}};
		//sphere_ = sphere
		break;
	case Collider::kPlane:
		break;
	case Collider::kAABB:
		/*aabb_ = std::make_unique<AABB>();
		aabb_->min = Subtract(worldTransform_->translation_, Multiply(size_, worldTransform_->scale_));
		aabb_->max = Add(worldTransform_->translation_, Multiply(size_, worldTransform_->scale_));*/
		HitBox_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
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

	HitBox_->SetCamera(camera);
	worldTransform_HitBox_.Initialize();
	worldTransform_HitBox_.scale_ = size;

}

//void Collider::Solve()
//{
//	/*if (fabs(velocity_.x) > 0.01f || fabs(velocity_.y) > 0.01f || fabs(velocity_.z) > 0.01f) {
//		float magnitude = miu_ * (mass_ * gravityScale_);
//
//		Vector3 direction = {};
//		if (velocity_.x != 0) {
//			direction.x = velocity_.x / std::fabs(velocity_.x);
//		}
//		if (velocity_.y != 0) {
//			direction.y = std::fabs(velocity_.y) / std::fabs(velocity_.y);
//		}
//		if (velocity_.z != 0) {
//			direction.z = velocity_.z / std::fabs(velocity_.z);
//		}
//		direction = Multiply(-1.0f, direction);
//
//		Vector3 frictionalForce = Multiply(magnitude, direction);
//		acceleration_ = Multiply(1.0f / mass_, frictionalForce);
//		acceleration_.y = -9.8f;
//
//		if (std::fabs(acceleration_.x) > std::fabs(velocity_.x)) {
//			acceleration_.x = velocity_.x;
//		}
//		if (std::fabs(acceleration_.z) > std::fabs(velocity_.z)) {
//			acceleration_.z = velocity_.z;
//		}
//		
//
//
//		velocity_ = Add(velocity_, Multiply(1.0f / 60.0f, acceleration_));
//
//		if (worldTransform_->translation_.y < 0.0f) {
//			worldTransform_->translation_.y = 0.0f;
//			velocity_.y = 0.0f;
//		}
//
//		worldTransform_->translation_ = Add(worldTransform_->translation_, Multiply(1.0f/ 60.0f, velocity_));
//		
//	}*/
//
//	if (mass_ != 0.0f) {
//		float magnitude = miu_ * (mass_ * gravityScale_);
//
//		Vector3 direction = {};
//		if (velocity_.x != 0) {
//			direction.x = velocity_.x / std::fabs(velocity_.x);
//		}
//		if (velocity_.y != 0) {
//			direction.y = std::fabs(velocity_.y) / std::fabs(velocity_.y);
//		}
//		if (velocity_.z != 0) {
//			direction.z = velocity_.z / std::fabs(velocity_.z);
//		}
//		direction = Multiply(-1.0f, direction);
//
//		Vector3 frictionalForce = Multiply(magnitude, direction);
//		acceleration_ = Multiply(1.0f / mass_, frictionalForce);
//		acceleration_.y = -9.8f;
//
//		if (std::fabs(acceleration_.x) > std::fabs(velocity_.x)) {
//			acceleration_.x = velocity_.x;
//		}
//		if (std::fabs(acceleration_.z) > std::fabs(velocity_.z)) {
//			acceleration_.z = velocity_.z;
//		}
//
//
//
//		velocity_ = Add(velocity_, Multiply(1.0f / 60.0f, acceleration_));
//
//		/*if (worldTransform_->translation_.y < 0.0f) {
//			worldTransform_->translation_.y = 0.0f;
//			velocity_.y = 0.0f;
//		}*/
//
//		worldTransform_->translation_ = Add(worldTransform_->translation_, Multiply(1.0f / 60.0f, velocity_));
//	}
//	
//	worldTransform_->UpdateMatrix(rotationType_);
//
//	switch (type_)
//	{
//	case Collider::kSphere:
//		sphere_->center = { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] };
//		break;
//	case Collider::kPlane:
//		break;
//	case Collider::kAABB:
//		aabb_->min = Subtract(Vector3(worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]), Multiply(1.25f, worldTransform_->scale_));
//		aabb_->max = Add(Vector3(worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]), Multiply(1.25f, worldTransform_->scale_));
//
//#ifdef _DEBUG
//		ImGui::Begin("AABB");
//		ImGui::SliderFloat3("min", &aabb_->min.x, -100.0f, 100.0f);
//		ImGui::SliderFloat3("max", &aabb_->max.x, -100.0f, 100.0f);
//		ImGui::End();
//
//#endif // _DEBUG
//		
//		break;
//	case Collider::kCapsule:
//		break;
//	case Collider::kOBB:
//		break;
//	default:
//		break;
//	}
//
//
//}



void Collider::HitBox()
{
	//worldTransform_HitBox_.scale_ = Multiply(2.5f, worldTransform_->scale_);
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

void Collider::HitBox(Camera* camera)
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

Vector3 Collider::GetHitBoxSize()
{
	Vector3 result;
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
		break;
	default:
		break;
	}

	return  Multiply(Multiply(0.5f, size_), worldTransform_->scale_);
}

AABB Collider::GetAABB()
{
	return AABB(Subtract(worldTransform_->translation_, GetHitBoxSize()), Add(worldTransform_->translation_, GetHitBoxSize()));
}

//void Collider::resolveCollision(AABB& b)
//{
//	// 各軸ごとに押し戻すベクトルを計算
//	float pushX = 0.0f, pushY = 0.0f, pushZ = 0.0f;
//
//	// X軸方向の押し戻し
//	if (aabb_->max.x > b.min.x && aabb_->min.x < b.min.x)
//		pushX = b.min.x - aabb_->max.x;
//	else if (aabb_->min.x < b.max.x && aabb_->max.x > b.max.x)
//		pushX = b.max.x - aabb_->min.x;
//
//	// Y軸方向の押し戻し
//	if (aabb_->max.y > b.min.y && aabb_->min.y < b.min.y)
//		pushY = b.min.y - aabb_->max.y;
//	else if (aabb_->min.y < b.max.y && aabb_->max.y > b.max.y)
//		pushY = b.max.y - aabb_->min.y;
//
//	// Z軸方向の押し戻し
//	if (aabb_->max.z > b.min.z && aabb_->min.z < b.min.z)
//		pushZ = b.min.z - aabb_->max.z;
//	else if (aabb_->min.z < b.max.z && aabb_->max.z > b.max.z)
//		pushZ = b.max.z - aabb_->min.z;
//
//	// 押し戻しベクトルを使用して位置を調整
//
//	if (pushY != 0.0f) {
//		worldTransform_->translation_.y += pushY;
//		if (pushY > 0) {
//			if (velocity_.y < 0) {
//				float re = velocity_.y * -1.0f;
//				velocity_.y += re;
//			}
//			//velocity_.y += 9.8f / 60.0f;
//
//		}
//		else {
//			velocity_.y = 0.0f;
//		}
//		
//	}
//	else {
//		worldTransform_->translation_.x += pushX;
//		//worldTransform_->translation_.y += pushY;
//		worldTransform_->translation_.z += pushZ;
//	}
//	
//
//	/*min.x += pushX;
//	min.y += pushY;
//	min.z += pushZ;
//	max.x += pushX;
//	max.y += pushY;
//	max.z += pushZ;*/
//}

