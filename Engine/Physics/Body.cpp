#include "Body.h"

namespace {
	Vector3 GetPushback(const AABB& a, const AABB& b) {

		float dx1 = b.max.x - a.min.x;
		float dx2 = a.max.x - b.min.x;
		float dy1 = b.max.y - a.min.y;
		float dy2 = a.max.y - b.min.y;
		float dz1 = b.max.z - a.min.z;
		float dz2 = a.max.z - b.min.z;

		float pushX = (dx1 < dx2) ? dx1 : -dx2;
		float pushY = (dy1 < dy2) ? dy1 : -dy2;
		float pushZ = (dz1 < dz2) ? dz1 : -dz2;

		if (std::abs(pushX) < std::abs(pushY) && std::abs(pushX) < std::abs(pushZ)) {
			return Vector3(pushX, 0, 0);
		}
		else if (std::abs(pushY) < std::abs(pushX) && std::abs(pushY) < std::abs(pushZ)) {
			return Vector3(0, pushY, 0);
		}
		else {
			return Vector3(0, 0, pushZ);
		}
	}
}

void Body::CreateBody(World* world, WorldTransform* worldTransform, float mass)
{
	mass_ = mass;
	velocity_ = { 0.0f, 0.0f, 0.0f };
	acceleration_ = { 0.0f, 0.0f, 0.0f };
	force_ = { 0.0f, 0.0f, 0.0f };
	frictionCombine_ = FrictionCombine::kNone;
	miu_ = 0.0f;
	magnitude_ = 0.0f;
	bounceCombine_ = BounceCombine::kNone;
	bounciness_ = 0.0f;
	normalVector_ = { 0.0f, 0.0f, 0.0f };
	world_ = world;
	worldTransform_ = worldTransform;
}

void Body::Solve()
{
	if (mass_ != 0.0f) {
		// 空気抵抗airResistanceは、速度に比例して逆方向に発生する
		Vector3 airResistance = Multiply(-drag_, velocity_);
		// airResistanceAccelerationはairResitanceからの加速度
		Vector3 airResistanceAcceleration = Multiply(1.0f / mass_, airResistance);

		Vector3 gravity = world_->GetGravity();
		if (gravityAcceleration_.Length() != 0.0f) {
			gravity = gravityAcceleration_;
		}

		if (frictionCombine_ != FrictionCombine::kNone && normalVector_.y != 0.0f) {
			Vector3 dirction = velocity_;
			dirction.y = 0.0f;
			dirction.Normalize();

			Vector3 frictionalForce = Multiply(-magnitude_, dirction);
			acceleration_ = Multiply(1.0f / mass_, frictionalForce);
			if (std::fabsf(acceleration_.x * world_->deltaTime_) > std::fabs(velocity_.x)) {
				acceleration_.x = velocity_.x / world_->deltaTime_;
			}
			if (std::fabsf(acceleration_.y * world_->deltaTime_) > std::fabs(velocity_.y)) {
				acceleration_.y = velocity_.y / world_->deltaTime_;
			}
			if (std::fabsf(acceleration_.z * world_->deltaTime_) > std::fabs(velocity_.z)) {
				acceleration_.z = velocity_.z / world_->deltaTime_;
			}

			acceleration_ = Add(acceleration_, Add(gravity, airResistanceAcceleration));
		}
		else {
			acceleration_ = Add(gravity, airResistanceAcceleration);
		}

		acceleration_ = Add(acceleration_, Multiply(1.0f / mass_, force_));

		force_ = { 0.0f, 0.0f, 0.0f };

		velocity_ = Add(velocity_, Multiply(world_->deltaTime_, acceleration_));

		worldTransform_->translation_ = Add(worldTransform_->translation_,Multiply(world_->deltaTime_, velocity_));
	}

	worldTransform_->UpdateMatrix();
}

Vector3 Body::RubberMovement(const Vector3& start, const Vector3& end, float limitLength, float stiffness, float dampingCoefficient)
{
	Vector3 diff = Subtract(start, end);
	float length = Length(diff);
	//length = 100f;
	if (length != 0.0f)
	{
		if (length > limitLength) {
			length - limitLength;
		}
		Vector3 direction = Normalize(diff);
		Vector3 displacement = Multiply(length, Subtract(start, end));
		Vector3 restoringForce = Multiply(-stiffness, displacement);
		Vector3 dampingForce = Multiply(-dampingCoefficient, velocity_);
		Vector3 force = Add(restoringForce, dampingForce);
		return force;
	}
	return { 0.0f, 0.0f, 0.0f };
}

void Body::AddForce(const Vector3& force, uint32_t mode)
{
	if (mode == 0) {
		force_ = Add(force_, force);
	}
	else {
		velocity_ = Add(velocity_, Multiply(1.0f / mass_, force));
	}
}

void Body::Reset()
{
	velocity_ = { 0.0f, 0.0f, 0.0f };
	acceleration_ = { 0.0f, 0.0f, 0.0f };

}

void Body::OnCollision(Body* body)
{
	if (mass_) {

		/*Vector3 dirction = Normalize(Subtract(worldTransform_->translation_, body->GetTranslation()));
		Vector3 resolve = Multiply(dirction, GetHitBoxSize());
		resolve = Multiply(resolve, Normalize(velocity_));
		resolve = Multiply(-1.0f, resolve);
		worldTransform_->translation_ = Add(worldTransform_->translation_, resolve);*/

		AABB a = { Subtract(GetMatWorldTranslation(), GetHitBoxSize()), Add(GetMatWorldTranslation(), GetHitBoxSize())};
		AABB b = { Subtract(body->GetMatWorldTranslation(), body->GetHitBoxSize()), Add(body->GetMatWorldTranslation(), body->GetHitBoxSize()) };
		Vector3 pushBack = GetPushback(a, b);
		worldTransform_->translation_ = Add(pushBack, GetMatWorldTranslation());
		//worldTransform_->UpdateMatrix();

		float miu = 0.0f;
		switch (frictionCombine_)
		{
		case Body::FrictionCombine::kNone:
			break;
		case Body::FrictionCombine::kAverage:
			miu = (miu_ + body->GetMiu()) / 2.0f;
			magnitude_ = miu * Length(Multiply(-mass_, world_->GetGravity()));
			break;
		case Body::FrictionCombine::kMinimum:
			miu = min(miu_, body->GetMiu());
			magnitude_ = miu * Length(Multiply(-mass_, world_->GetGravity()));
			break;
		case Body::FrictionCombine::kMaximum:
			miu = max(miu_, body->GetMiu());
			magnitude_ = miu * Length(Multiply(-mass_, world_->GetGravity()));
			break;
		case Body::FrictionCombine::kMultiply:
			miu = miu_ * body->GetMiu();
			magnitude_ = miu * Length(Multiply(-mass_, world_->GetGravity()));
			break;
		}

		float e = 0.0f;
		switch (bounceCombine_)
		{
		case Body::BounceCombine::kNone:
			if (pushBack.x != 0.0f) {
				velocity_.x = 0.0f;
			}
			if (pushBack.y != 0.0f) {
				velocity_.y = 0.0f;
			}
			break;
		case Body::BounceCombine::kAverage:
			e = (bounciness_ + body->GetBounciness()) / 2.0f;
			break;
		case Body::BounceCombine::kMinimum:
			e = min(bounciness_, body->GetBounciness());
			break;
		case Body::BounceCombine::kMaximum:
			e = max(bounciness_, body->GetBounciness());
			break;
		case Body::BounceCombine::kMultiply:
			e = bounciness_ * body->GetBounciness();
			break;
		}

		if (bounceCombine_ != BounceCombine::kNone) {
			if (pushBack.x != 0.0f) {
				velocity_.x = -velocity_.x * e;
			}
			if (pushBack.y != 0.0f) {
				velocity_.y = -velocity_.y * e;
			}
			if (pushBack.z != 0.0f) {
				velocity_.z = -velocity_.z * e;
			}
		}

		normalVector_ = pushBack;
		vertical_ = pushBack;
	}
}


