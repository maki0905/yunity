#include "Body.h"


void Body::CreateBody(World* world, WorldTransform* worldTransform, float mass)
{
	mass_ = mass;
	velocity_ = { 0.0f, 0.0f, 0.0f };
	acceleration_ = { 0.0f, 0.0f, 0.0f };
	force_ = { 0.0f, 0.0f, 0.0f };
	world_ = world;
	worldTransform_ = worldTransform;
}

void Body::Solve()
{
	// 空気抵抗airResistanceは、速度に比例して逆方向に発生する
	Vector3 airResistance = Multiply(-drag_, velocity_);
	// airResistanceAccelerationはairResitanceからの加速度
	Vector3 airResistanceAcceleration = Multiply(1.0f / mass_, airResistance);

	Vector3 gravity = world_->GetGravity();
	if (gravityAcceleration_.Length() != 0.0f) {
		gravity = gravityAcceleration_;
	}

	acceleration_ = Add(gravity, airResistanceAcceleration);
	acceleration_ = Add(acceleration_, Multiply(1.0f / mass_, force_));
	force_ = { 0.0f, 0.0f, 0.0f };
	
	velocity_ = Add(velocity_, Multiply(world_->deltaTime_, acceleration_));

	worldTransform_->translation_ = Add(worldTransform_->translation_, velocity_);

	worldTransform_->UpdateMatrix();
}

Vector3 Body::RubberMovement(const Vector3& start, const Vector3& end, float limitLength, float stiffness, float dampingCoefficient)
{
	Vector3 diff = Subtract(start , end);
	float length = Length(diff);
	//length = 100f;
	if (length != 0.0f)
	{
		if (length > limitLength) {
			length - limitLength;
		}
		Vector3 direction = Normalize(diff);
		Vector3 displacement = Multiply(length, Subtract(start,end));
		Vector3 restoringForce = Multiply(-stiffness, displacement);
		Vector3 dampingForce = Multiply(-dampingCoefficient, velocity_);
		Vector3 force = Add(restoringForce , dampingForce);
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
