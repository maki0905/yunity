#include "World.h"

#include "DirectXCore.h"
#include "ImGuiManager.h"
#include "Joint.h"
#include "Object3D.h"

void yunity::World::Initialize(const Vector3& gravity)
{
	collisionManager_ = std::make_unique<CollisionManager>();
	objectList_.clear();
	jointList_.clear();
	gravity_ = gravity;
	lastTime_ = std::chrono::high_resolution_clock::now();
	isFixedTime_ = true;
}

void yunity::World::Solve()
{
	float time = 0.0f;

	// 時間計測
	if (!isFixedTime_) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - lastTime_;
		deltaTime_ = deltaTime.count();
		lastTime_ = currentTime;
	}
	else {
		time = fixedDeltaTime_;
	}

	// ジョイント解決
	for (auto& joint : jointList_) {
		joint->Solve();
	}

	// 物理演算
	for (auto& obj : objectList_) {
		//obj->Solve(time);

		for (int i = 0; i < 8; i++) {
			obj->SolveVelocity(time / 8.0f);
		}
		for (int i = 0; i < 5; i++) {
			obj->SolvePosition(time / 5.0f);
		}
	}

	// 衝突判定
	collisionManager_->ClearCollider();
	for (auto& collider : objectList_) {
		collisionManager_->SetCollider(collider);
	}
	collisionManager_->CheckAllCollision(this);

	SolveConstraints();

	// 衝突解決
	/*for (auto& obj : objectList_) {
		obj->SolveConstraints();
	}*/
}

void yunity::World::SolveConstraints()
{

	for (auto& info : PersistentManifolds_) {
		float totalMass = info.colliderA->GetMass() + info.colliderB->GetMass();

		if (totalMass == 0.0f) { // 質量が0の場合は処理しない
			continue;
		}

		Vector3 angularVelocityA = { 0.0f, 0.0f, 0.0f };
		Vector3 angularVelocityB = { 0.0f, 0.0f, 0.0f };

		Vector3 inertiaTensorA = { 0.0f, 0.0f, 0.0f };
		Vector3 inertiaTensorB = { 0.0f, 0.0f, 0.0f };

		// 相対速度を求める
		if (info.colliderA->GetMass() != 0.0f) { // 質量が0の場合は計算しない
			angularVelocityA = Cross(info.colliderA->GetAngularVelocity(), info.localPointA);
			inertiaTensorA = Cross(TransformVector3(Cross(info.localPointA, info.contactNormal), info.colliderA->GetInertiaTensor()), info.localPointA);
		}
		if (info.colliderB->GetMass() != 0.0f) { // 質量が0の場合は計算しない
			angularVelocityB = Cross(info.colliderB->GetAngularVelocity(), info.localPointB);
			inertiaTensorB = Cross(TransformVector3(Cross(info.localPointB, info.contactNormal), info.colliderB->GetInertiaTensor()), info.localPointB);
		}
		float angularEffect = Dot(Add(inertiaTensorA, inertiaTensorB), info.contactNormal);
		Vector3 fullVelocityA = Add(info.colliderA->GetVelocity(), angularVelocityA);
		Vector3 fullVelocityB = Add(info.colliderB->GetVelocity(), angularVelocityB);
		Vector3 relativeVelocity = Subtract(fullVelocityA, fullVelocityB);
		float velocityAlongNormal = Dot(relativeVelocity, info.contactNormal);


		///　途中
		float restitutionA = info.colliderA->GetRestitution(info.colliderB->GetBounciness());
		float restitutionB = info.colliderB->GetRestitution(info.colliderA->GetBounciness());
		float restitution = 0.5f * (restitutionA + restitutionB);

		float impulseMag = -(1.0f + restitution) * velocityAlongNormal / (totalMass + angularEffect);
		Vector3 impulse = Multiply(impulseMag, info.contactNormal);

		if (info.colliderA->GetMass() > 0.0f) {
			// 位置補正
			info.colliderA->PositionalCorrection(totalMass, -info.penetrationDepth, info.contactNormal);
			// 反発力を適用
			info.colliderA->AddForce(impulse, Body::ForceMode::kImpulse);
			info.colliderA->AddTorque(Cross(info.localPointA, impulse), Body::ForceMode::kImpulse);
		}

		if (info.colliderB->GetMass() > 0.0f) {
			// 位置補正
			info.colliderB->PositionalCorrection(totalMass, -info.penetrationDepth, info.contactNormal);
			// 反発力を適用
			info.colliderB->AddForce(Multiply(-1.0f, impulse), Body::ForceMode::kImpulse);
			info.colliderB->AddTorque(Cross(info.localPointB, Multiply(-1.0f, impulse)), Body::ForceMode::kImpulse);
		}
		///

		//if (info.colliderA->GetMass() != 0.0f || info.colliderB->GetMass() != 0.0f) {
		//	//float restitution = std::min(info.colliderA->GetRestitution(), info.colliderB->GetRestitution())
		//}

		//if (info.colliderA->GetMass() != 0.0f) {
		//	// 位置補正
		//	info.colliderA->PositionalCorrection(totalMass, -info.penetrationDepth, info.contactNormal);

		//	float restitution = info.colliderA->GetRestitution(info.colliderB->GetBounciness());

		//	float impulseMagnitude = (-(1.0f + restitution) * velocityAlongNormal / (totalMass + angularEffect));

		//	Vector3 impulse = Multiply(impulseMagnitude, info.contactNormal);

		//	// 反発力を適用
		//	info.colliderA->AddForce(impulse, Body::ForceMode::kImpulse);
		//	info.colliderA->AddTorque(Cross(info.localPointA, impulse), Body::ForceMode::kImpulse);
		//}
		//if (info.colliderB->GetMass() != 0.0f) {
		//	// 位置補正
		//	info.colliderB->PositionalCorrection(totalMass, info.penetrationDepth, info.contactNormal);

		//	
		//	float restitution = info.colliderB->GetRestitution(info.colliderA->GetBounciness());

		//	float impulseMagnitude = (-(1.0f + restitution) * velocityAlongNormal / (totalMass + angularEffect));

		//	Vector3 impulse = Multiply(impulseMagnitude, info.contactNormal);

		//	// 反発力を適用
		//	info.colliderB->AddForce(Multiply(-1.0f, impulse), Body::ForceMode::kImpulse);
		//	info.colliderB->AddTorque(Cross(info.localPointB, impulse), Body::ForceMode::kImpulse);
		//}
		

	}
	PersistentManifolds_.clear();
}

void yunity::World::TakeObject(Object3D* collider)
{
	for (std::list<Object3D*>::iterator iterator = objectList_.begin(); iterator != objectList_.end();) {
		if (*iterator == collider) {
			iterator = objectList_.erase(iterator);
			continue;
		}

		iterator++;
	}

}

void yunity::World::AddJoint(Joint* joint)
{
	bool duplicationCheck = true;
	for (std::list<Joint*>::iterator iterator = jointList_.begin(); iterator != jointList_.end();) {
		if (*iterator == joint) {
			duplicationCheck = false;
		}
		iterator++;
	}
	if (duplicationCheck) {
		jointList_.emplace_back(joint);
	}
}

void yunity::World::TakeJoint(Joint* joint)
{
	for (std::list<Joint*>::iterator iterator = jointList_.begin(); iterator != jointList_.end();) {
		if (*iterator == joint) {
			iterator = jointList_.erase(iterator);
			continue;
		}

		iterator++;
	}
}
