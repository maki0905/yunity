#include "Body.h"

#include <cmath>

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

	Vector3 GetPushback(const Sphere& sphere, const AABB& aabb) {
		// 最近接点を求める
		Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

		Vector3 delta = Subtract(sphere.center, closestPoint);
		float distance = Length(delta);

		float penetrationDepth = sphere.radius - distance;
		Vector3 normal = delta.Normalize();
		return Multiply(penetrationDepth, normal);
	}

	Vector3 GetPushback(const AABB& aabb, const Sphere& sphere) {
		// 最近接点を求める
		Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

		/*Vector3 delta = Subtract(sphere.center, closestPoint);*/
		Vector3 delta = Subtract(closestPoint, sphere.center);
		float distance = Length(delta);

		float penetrationDepth = sphere.radius - distance;

		Vector3 normal = delta.Normalize();
		return Multiply(penetrationDepth, normal);
	}

	float GetProjectionRadius(const OBB& obb, const Vector3& axis) {
		float result;
		result =
			obb.size.x * std::fabsf(Dot(obb.orientations[0], axis)) +
			obb.size.y * std::fabsf(Dot(obb.orientations[1], axis)) +
			obb.size.z * std::fabsf(Dot(obb.orientations[2], axis));
		return result;
	}

	Vector3 GetPushback(const OBB& obb1, const OBB& obb2) {
		float minPenetrationDepth = (std::numeric_limits<float>::max)();
		Vector3 minPenetrationAxis;

		Vector3 axes[15];  // 衝突判定に使用する分離軸
		int axisCount = 0;

		// OBBの各軸を分離軸に追加
		for (int i = 0; i < 3; i++) {
			axes[axisCount++] = obb1.orientations[i];
			axes[axisCount++] = obb2.orientations[i];
		}

		// OBBの各軸の外積を分離軸に追加
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				Vector3 crossAxis = Cross(obb1.orientations[i], obb2.orientations[j]);
				if (Length(crossAxis) > 1e-6) {
					axes[axisCount++] = Normalize(crossAxis);
				}
			}
		}

		//Vector3 centerOffset = Subtract(obb2.center, obb1.center);
		Vector3 centerOffset = Subtract(obb1.center, obb2.center);
		// 各軸に対して衝突判定
		for (int i = 0; i < axisCount; i++) {
			Vector3 axis = axes[i];

			// OBB1とOBB2の軸への投影半径を計算
			float r1 = GetProjectionRadius(obb1, axis);
			float r2 = GetProjectionRadius(obb2, axis);
			float distance = std::fabs(Dot(centerOffset, axis));

			// 貫通深度を計算
			float penetrationDepth = (r1 + r2) - distance;

			if (penetrationDepth < 0.0f) {
				continue;
			}

			// 最小の貫通深度を保存
			if (penetrationDepth < minPenetrationDepth) {
				minPenetrationDepth = penetrationDepth;
				minPenetrationAxis = Normalize(axis);
			}
		}

		// 中心オフセットとの内積を使用して、押し返し方向を決定
		if (Dot(centerOffset, minPenetrationAxis) < 0) {
			minPenetrationAxis = Multiply(-1, minPenetrationAxis);
		}

		// Y軸方向の押し返しを優先
		if (std::fabs(minPenetrationAxis.y) > std::fabs(minPenetrationAxis.x) &&
			std::fabs(minPenetrationAxis.y) > std::fabs(minPenetrationAxis.z)) {
			minPenetrationAxis = Vector3(0, minPenetrationAxis.y, 0);
		}
		// 押し返しベクトルを計算して返す
		return Multiply(minPenetrationDepth, minPenetrationAxis);
	}

	//Vector3 GetPushback(const OBB& obb1, const OBB& obb2) {
	//	float minPenetrationDepth = (std::numeric_limits<float>::max)();
	//	Vector3 minPenetrationAxis;

	//	Vector3 axes[15];
	//	int index = 0;

	//	for (int i = 0; i < 3; i++) {
	//		axes[index++] = obb1.orientations[i];
	//		axes[index++] = obb2.orientations[i];
	//	}

	//	for (int i = 0; i < 3; i++) {
	//		for (int j = 0; j < 3; j++) {
	//			Vector3 crossAxis = Vector3(
	//				obb1.orientations[i].y * obb2.orientations[j].z - obb1.orientations[i].z * obb2.orientations[j].y,
	//				obb1.orientations[i].z * obb2.orientations[j].x - obb1.orientations[i].x * obb2.orientations[j].z,
	//				obb1.orientations[i].x * obb2.orientations[j].y - obb1.orientations[i].y * obb2.orientations[j].x
	//			);
	//			if (crossAxis.Length() > 1e-6) {
	//				axes[index++] = crossAxis.Normalize();
	//			}

	//		}
	//	}

	//	Vector3 centerOffset = Subtract(obb2.center, obb1.center);

	//	for (int i = 0; i < index; i++) {
	//		Vector3 axis = axes[i];

	//		float r1 = obb1.size.x * std::fabsf(Dot(obb1.orientations[0], axis)) + obb1.size.y * std::fabsf(Dot(obb1.orientations[1], axis)) + obb1.size.z * std::fabsf(Dot(obb1.orientations[2], axis));
	//		float r2 = obb2.size.x * std::fabsf(Dot(obb2.orientations[0], axis)) + obb2.size.y * std::fabsf(Dot(obb2.orientations[1], axis)) + obb2.size.z * std::fabsf(Dot(obb2.orientations[2], axis));
	//		float distance = std::fabsf(Dot(Subtract(obb2.center, obb1.center), axis));

	//		if (distance > r1 + r2) {
	//			return Vector3(0, 0, 0);
	//		}

	//		float penetrationDepth = (r1 + r2) - distance;

	//		if (penetrationDepth < minPenetrationDepth) {
	//			minPenetrationDepth = penetrationDepth;
	//			minPenetrationAxis = axis.Normalize();
	//		}

	//	}

	//	return Multiply(minPenetrationDepth, minPenetrationAxis);
	//}

	// 分離軸に基づく投影範囲を計算
	void ProjectOBBOnAxis(const OBB& obb, const Vector3& axis, float& minProj, float& maxProj) {
		// OBBの中心の投影
		float centerProjection = Dot(obb.center, axis);
		// OBBの各軸方向の半径の合計を計算
		float extent =
			obb.size.x * std::fabsf(Dot(obb.orientations[0], axis)) +
			obb.size.y * std::fabsf(Dot(obb.orientations[1], axis)) +
			obb.size.z * std::fabsf(Dot(obb.orientations[2], axis));
		// OBBの投影範囲を決定
		minProj = centerProjection - extent;
		maxProj = centerProjection + extent;
	}
	// OBB同士の最も近い接触点を計算する関数
	Vector3 GetContactPoint(const OBB& obb1, const OBB& obb2) {
		Vector3 minPenetrationAxis;
		float minPentrationDepth = (std::numeric_limits<float>::max)();

		// OBBの軸とその外積を分離軸として使用
		Vector3 testAxes[] = {
			obb1.orientations[0], obb1.orientations[1], obb1.orientations[2],
			obb2.orientations[0], obb2.orientations[1], obb2.orientations[2],
			Cross(obb1.orientations[0], obb2.orientations[0]),
			Cross(obb1.orientations[0], obb2.orientations[1]),
			Cross(obb1.orientations[0], obb2.orientations[2]),
			Cross(obb1.orientations[1], obb2.orientations[0]),
			Cross(obb1.orientations[1], obb2.orientations[1]),
			Cross(obb1.orientations[1], obb2.orientations[2]),
			Cross(obb1.orientations[2], obb2.orientations[0]),
			Cross(obb1.orientations[2], obb2.orientations[1]),
			Cross(obb1.orientations[2], obb2.orientations[2]),
		};
		// 各分離軸について検証
		for (const Vector3& axis : testAxes) {
			// 軸の長さが非常に小さい場合は無視
			if (axis.Length() < 1e-6) {
				continue;
			}

			// OBB1とOBB2をこの軸に投影して範囲を取得
			float min1, max1, min2, max2;
			ProjectOBBOnAxis(obb1, axis, min1, max1);
			ProjectOBBOnAxis(obb2, axis, min2, max2);

			// 投影範囲が分離している場合は衝突していない
			if (max1 < min2 || max2 < min1) {
				return Vector3();// 接触していない
			}

			// 重なりの深さを計算
			float penetrationDepth = min(max1, max2) - max(min1, min2);

			// 最小の重なりを保存
			if (penetrationDepth < minPentrationDepth) {
				minPentrationDepth = penetrationDepth;
				minPenetrationAxis = Normalize(axis);
			}
		}
		// 接触点は最小分離軸に基づく
		return Multiply(minPentrationDepth, minPenetrationAxis);
	}

	Body::PersistentManifold* GetNewManifold(Body* bodyA, Body* bodyB) {
		Body::PersistentManifold* persistentManifold = new Body::PersistentManifold;

		// ボディー
		persistentManifold->bodyA = bodyA;
		persistentManifold->bodyB = bodyB;

		// 接触点での法線ベクトル
		Matrix4x4 rotateA = MakeRotateXYZMatrix(bodyA->GetWorldTransform()->rotation_);
		Vector3 orientationsA[3] = { {rotateA.m[0][0], rotateA.m[0][1], rotateA.m[0][2]}, {rotateA.m[1][0], rotateA.m[1][1], rotateA.m[1][2]}, {rotateA.m[2][0], rotateA.m[2][1], rotateA.m[2][2]} };
		Matrix4x4 rotateB = MakeRotateXYZMatrix(bodyB->GetWorldTransform()->rotation_);
		Vector3 orientationsB[3] = { {rotateB.m[0][0], rotateB.m[0][1], rotateB.m[0][2]}, {rotateB.m[1][0], rotateB.m[1][1], rotateB.m[1][2]}, {rotateB.m[2][0], rotateB.m[2][1], rotateB.m[2][2]} };
		persistentManifold->contactNormal = GetPushback(OBB{ bodyA->GetMatWorldTranslation(), orientationsA[0], orientationsA[1], orientationsA[2], bodyA->GetHitBoxSize() }, OBB{ bodyB->GetMatWorldTranslation(), orientationsB[0], orientationsB[1], orientationsB[2], bodyB->GetHitBoxSize() }).Normalize();
		
		// 接触点
		persistentManifold->contactPoint = GetContactPoint(OBB{ bodyA->GetMatWorldTranslation(), orientationsA[0], orientationsA[1], orientationsA[2], bodyA->GetHitBoxSize() }, OBB{ bodyB->GetMatWorldTranslation(), orientationsB[0], orientationsB[1], orientationsB[2], bodyB->GetHitBoxSize() });

		// 位置ベクトル X 法線ベクトル
		persistentManifold->crossNormal = Cross(Subtract(persistentManifold->contactPoint, bodyA->GetMatWorldTranslation()), persistentManifold->contactNormal);

		return persistentManifold;
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

void Body::Solve(float time)
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
			if (std::fabsf(acceleration_.x * time) > std::fabs(velocity_.x)) {
				acceleration_.x = -velocity_.x / time;
			}
			if (std::fabsf(acceleration_.y * time) > std::fabs(velocity_.y)) {
				acceleration_.y = -velocity_.y / time;
			}
			if (std::fabsf(acceleration_.z * time) > std::fabs(velocity_.z)) {
				acceleration_.z = -velocity_.z / time;
			}

			acceleration_ = Add(acceleration_, Add(gravity, airResistanceAcceleration));
		}
		else {
			acceleration_ = Add(gravity, airResistanceAcceleration);
		}

		if (inertiaMoment_ != 0.0f) {
			Vector3 airResistanceTorque = Multiply(-angularDrag_, angularVelocity_);

			Vector3 airResistanceAngularAcceleration = Multiply(1.0 / inertiaMoment_, airResistanceTorque);

			//if (frictionCombine_ != FrictionCombine::kNone && normalVector_.y != 0.0f) {
			//	Vector3 dirction = angularVelocity_;
			//	dirction.Normalize();

			//	Vector3 frictionalForce = Multiply(-magnitude_, dirction);
			//	angularAcceleration_ = Multiply(1.0f / mass_, frictionalForce);
			//	if (std::fabsf(angularAcceleration_.x * time) > std::fabs(angularVelocity_.x)) {
			//		angularAcceleration_.x = angularVelocity_.x / time;
			//	}
			//	if (std::fabsf(angularAcceleration_.y * time) > std::fabs(angularVelocity_.y)) {
			//		angularAcceleration_.y = angularVelocity_.y / time;
			//	}
			//	if (std::fabsf(angularAcceleration_.z * time) > std::fabs(angularVelocity_.z)) {
			//		angularAcceleration_.z = angularVelocity_.z / time;
			//	}
			//}

			angularAcceleration_ = Add(angularAcceleration_, airResistanceAngularAcceleration);
			if (std::fabsf(angularAcceleration_.x * time) > std::fabs(angularVelocity_.x)) {
				angularAcceleration_.x = angularVelocity_.x / time;
			}
			else if (std::fabsf(angularVelocity_.x) < 0.01f) {
				angularAcceleration_.x = 0.0f;
				angularVelocity_.x = 0.0f;
			}
			if (std::fabsf(angularAcceleration_.y * time) > std::fabs(angularVelocity_.y)) {
				angularAcceleration_.y = angularVelocity_.y / time;
			}
			else if (std::fabsf(angularVelocity_.y) < 0.01f) {
				angularAcceleration_.y = 0.0f;
				angularVelocity_.y = 0.0f;
			}
			if (std::fabsf(angularAcceleration_.z * time) > std::fabs(angularVelocity_.z)) {
				angularAcceleration_.z = angularVelocity_.z / time;
			}
			else if (std::fabsf(angularVelocity_.z) < 0.01f) {
				angularAcceleration_.z = 0.0f;
				angularVelocity_.z = 0.0f;
			}

			angularAcceleration_ = Add(angularAcceleration_, Multiply(1.0f / inertiaMoment_, torque_));
			torque_ = { 0.0f, 0.0f, 0.0f };
			angularVelocity_ = Add(angularVelocity_, Multiply(time, angularAcceleration_));
			worldTransform_->rotation_ = Add(worldTransform_->rotation_, Multiply(time, angularVelocity_));
		}

		acceleration_ = Add(acceleration_, Multiply(1.0f / mass_, force_));

		force_ = { 0.0f, 0.0f, 0.0f };

		velocity_ = Add(velocity_, Multiply(time, acceleration_));

		worldTransform_->translation_ = Add(worldTransform_->translation_, Multiply(time, velocity_));

		magnitude_ = 0.0f;
	}

	worldTransform_->UpdateMatrix();
}

void Body::SolveConstraints()
{
	Vector3 L = GetHitBoxSize();
	inertiaTensor_.m[0][0] = (1.0f / 12.0f) * mass_ * (L.y * L.y + L.z * L.z);
	inertiaTensor_.m[1][1] = (1.0f / 12.0f) * mass_ * (L.x * L.x + L.z * L.z);
	inertiaTensor_.m[2][2] = (1.0f / 12.0f) * mass_ * (L.x * L.x + L.y * L.y);
	inertiaTensor_.m[0][1] = inertiaTensor_.m[1][0] = 0.0f;
	inertiaTensor_.m[0][2] = inertiaTensor_.m[2][0] = 0.0f;
	inertiaTensor_.m[1][2] = inertiaTensor_.m[2][1] = 0.0f;

	Matrix3x3 R = MakeRotateMatrix(worldTransform_->rotation_);
	Matrix3x3 Rt = Transpose(R);

	inertiaTensor_ = Multiply(Multiply(R, inertiaTensor_), Rt);

	for (auto& c : persistentManifold_) {

		Vector3 relativeVelocity = Subtract(c->bodyA->velocity_, c->bodyB->velocity_);
		float velocityAlongNormal = Dot(relativeVelocity, c->contactNormal);
		float impulseMagnitude = -0.01f * velocityAlongNormal / (1.0f / c->bodyA->mass_);
		Vector3 impulse = Multiply(impulseMagnitude, c->contactNormal);
		AddImpulse(impulse, c->contactPoint);
		//AddTorque(TransformVector3(Cross(Subtract(c->contactPoint, c->bodyA->worldTransform_->GetMatWorldTranslation()), impulse), inertiaTensor), 1);
		//angularVelocity_ = Add(angularVelocity_, TransformVector3(Cross(Subtract(c->contactPoint, c->bodyA->worldTransform_->GetMatWorldTranslation()), impulse), inertiaTensor));
	}

	/*if (pushback_.x != 0.0f) {
		velocity_.x = -velocity_.x * restitution_;
	}
	if (pushback_.y != 0.0f) {
		velocity_.y = -velocity_.y * restitution_;
	}
	if (pushback_.z != 0.0f) {
		velocity_.z = -velocity_.z * restitution_;
	}*/

	worldTransform_->translation_ = Add(pushback_, worldTransform_->translation_);
	worldTransform_->UpdateMatrix();
	pushback_.SetZero();
}


Vector3 Body::RubberMovement(const Vector3& start, const Vector3& end, float limitLength, float stiffness, float dampingCoefficient)
{
	Vector3 diff = Subtract(start, end);
	float length = Length(diff);
	//length = 100f;
	if (length != 0.0f)
	{
		if (length > limitLength) {
			length = limitLength;
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

Vector3 Body::Spring(const Vector3& anchor, const Vector3& position, float naturalLength, float stiffness, float dampingCoefficient)
{
	Vector3 diff = Subtract(position, anchor);
	float length = Length(diff);
	//length = 100f;
	if (length != 0.0f)
	{
		Vector3 direction = Normalize(diff);
		Vector3 restPosition = Add(anchor, Multiply(naturalLength, direction));
		Vector3 displacement = Multiply(length, Subtract(position, restPosition));
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

void Body::AddTorque(const Vector3& torque, uint32_t mode)
{
	/*if (mode == 0) {
		torque_ = Add(torque_, Multiply(DegToRad(), torque));
	}
	else {
		angularVelocity_ = Add(angularVelocity_, Multiply(angularDrag_, Multiply(DegToRad(), torque)));
	}*/
	if (mode == 0) {
		torque_ = Add(torque_, torque);
	}
	else {
		angularVelocity_ = Add(angularVelocity_, Multiply(angularDrag_, torque));
	}
}

void Body::AddForce(const Vector3& force, const Vector3& point)
{
	force_ = Add(force_, force);
	torque_ = Add(torque_, Cross(Subtract(point, GetMatWorldTranslation()),force));

}

void Body::AddImpulse(const Vector3& impulse, const Vector3& pos)
{
	velocity_ = Add(velocity_, Multiply(1.0f / mass_, impulse));
	Vector3 torque = Cross(Subtract(pos, GetMatWorldTranslation()), impulse);
	Matrix3x3 ii = Inverse(inertiaTensor_);
	angularVelocity_ = Add(angularVelocity_, TransformVector3(torque, ii));
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
		Vector3 pushback = { 0.0f, 0.0f, 0.0f };
		switch (GetShape())
		{
		case Collider::Shape::kSphere:
			switch (body->GetShape())
			{
			case Collider::Shape::kSphere:
				//pushback = GetPushback(Sphere{worldTransform_->GetMatWorldTranslation(), GetHitBoxSize().x * 2.0f},);
				break;
			case Collider::Shape::kAABB:
				pushback = GetPushback(Sphere{ worldTransform_->GetMatWorldTranslation(), GetHitBoxSize().x * 2.0f }, AABB{ Subtract(body->GetMatWorldTranslation(), body->GetHitBoxSize()), Add(body->GetMatWorldTranslation(), body->GetHitBoxSize()) });

				break;
			}
			break;
		case Collider::Shape::kAABB:
			switch (body->GetShape())
			{
			case Collider::Shape::kSphere:
				pushback = GetPushback(AABB{ Subtract(GetMatWorldTranslation(), GetHitBoxSize()), Add(GetMatWorldTranslation(), GetHitBoxSize()) }, Sphere{ body->GetMatWorldTranslation(), body->GetHitBoxSize().x * 2.0f });
				break;
			case Collider::Shape::kAABB:
				pushback = GetPushback(AABB{ Subtract(GetMatWorldTranslation(), GetHitBoxSize()), Add(GetMatWorldTranslation(), GetHitBoxSize()) }, AABB{ Subtract(body->GetMatWorldTranslation(), body->GetHitBoxSize()), Add(body->GetMatWorldTranslation(), body->GetHitBoxSize()) });
				break;
			}

			break;
		case Collider::Shape::kOBB:
			switch (body->GetShape())
			{
			case Collider::Shape::kSphere:
				pushback = GetPushback(AABB{ Subtract(GetMatWorldTranslation(), GetHitBoxSize()), Add(GetMatWorldTranslation(), GetHitBoxSize()) }, Sphere{ body->GetMatWorldTranslation(), body->GetHitBoxSize().x * 2.0f });
				break;
			case Collider::Shape::kAABB:
				pushback = GetPushback(AABB{ Subtract(GetMatWorldTranslation(), GetHitBoxSize()), Add(GetMatWorldTranslation(), GetHitBoxSize()) }, AABB{ Subtract(body->GetMatWorldTranslation(), body->GetHitBoxSize()), Add(body->GetMatWorldTranslation(), body->GetHitBoxSize()) });
				break;
			case Collider::Shape::kOBB:
				Matrix4x4 rotateA = MakeRotateXYZMatrix(GetWorldTransform()->rotation_);
				Vector3 orientationsA[3] = { {rotateA.m[0][0], rotateA.m[0][1], rotateA.m[0][2]}, {rotateA.m[1][0], rotateA.m[1][1], rotateA.m[1][2]}, {rotateA.m[2][0], rotateA.m[2][1], rotateA.m[2][2]} };
				Matrix4x4 rotateB = MakeRotateXYZMatrix(body->GetWorldTransform()->rotation_);
				Vector3 orientationsB[3] = { {rotateB.m[0][0], rotateB.m[0][1], rotateB.m[0][2]}, {rotateB.m[1][0], rotateB.m[1][1], rotateB.m[1][2]}, {rotateB.m[2][0], rotateB.m[2][1], rotateB.m[2][2]} };
				pushback = GetPushback(OBB{ GetMatWorldTranslation(), orientationsA[0], orientationsA[1], orientationsA[2], GetHitBoxSize() }, OBB{ body->GetMatWorldTranslation(), orientationsB[0], orientationsB[1], orientationsB[2], body->GetHitBoxSize() });
				if (body->mass_ != 0.0f) {
					pushback = Multiply(0.5f, pushback);
				}
				pushback_ = Add(pushback_, pushback);
				
				persistentManifold_.emplace_back(GetNewManifold(this, body));
				break;
			}
			break;
		}

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
			restitution_ = e;
			break;
		case Body::BounceCombine::kAverage:
			e = (bounciness_ + body->bounciness_) / 2.0f;
			break;
		case Body::BounceCombine::kMinimum:
			e = min(bounciness_, body->bounciness_);
			break;
		case Body::BounceCombine::kMaximum:
			e = max(bounciness_, body->bounciness_);
			break;
		case Body::BounceCombine::kMultiply:
			e = bounciness_ * body->bounciness_;
			break;
		}
		if (restitution_ < e) {
			restitution_ = e;
		}
		normalVector_ = pushback;
		vertical_ = pushback;
	}
}


