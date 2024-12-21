#pragma once

#include "Collider.h"
#include "WorldTransform.h"
#include "Shape.h"

namespace yunity {
	class World;
}

namespace yunity {

	/*
	* @brief ボディークラス
	*/
	class Body : public Collider {
	public:
		enum class ForceMode {
			kForce,
			kImpulse,
		};

		enum class FrictionCombine {
			kNone,    // 計算しない
			kAverage, // 平均化
			kMinimum, // 小さい方 
			kMaximum, // 大きい方
			kMultiply // 乗算
		};

		enum class BounceCombine {
			kNone,    // 計算しない
			kAverage, // 平均化
			kMinimum, // 小さい方 
			kMaximum, // 大きい方
			kMultiply // 乗算
		};

		struct PersistentManifold {
			float massA;
			float massB;
			float restitution;
			float friction;
			Vector3 penetrationDepth;
			Vector3 velocityA;
			Vector3 velocityB;
			Vector3 angularVelocityA;
			Vector3 angularVelocityB;
			Matrix3x3 inversInertiaTensorA;
			Matrix3x3 inversInertiaTensorB;
			Vector3 contactPoint;
			Vector3 contactNormal;
			Vector3 crossNormal;
		};

	public:
		/// <summary>
		/// ボディ生成
		/// </summary>
		/// <param name="world"></param>
		/// <param name="worldTransform"></param>
		/// <param name="mass"></param>
		void CreateBody(World* world, WorldTransform* worldTransform, float mass = 0.0f);

		/// <summary>
		/// 解決
		/// </summary>
		/// <param name="time"></param>
		void Solve(float time);

		/// <summary>
		/// 衝突解決
		/// </summary>
		void SolveConstraints(/*float time*/);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <param name="limitLength"></param>
		/// <param name="stiffness"></param>
		/// <param name="dampingCoefficient"></param>
		/// <returns></returns>
		Vector3 RubberMovement(const Vector3& start, const Vector3& end, float limitLength, float stiffness, float dampingCoefficient);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="anchor"></param>
		/// <param name="position"></param>
		/// <param name="naturalLength"></param>
		/// <param name="stiffness"></param>
		/// <param name="dampingCoefficient"></param>
		/// <returns></returns>
		Vector3 Spring(const Vector3& anchor, const Vector3& position, float naturalLength, float stiffness, float dampingCoefficient);

		/// <summary>
		/// 力の適用
		/// </summary>
		/// <param name="force"></param>
		/// <param name="mode"></param>
		void AddForce(const Vector3& force, ForceMode mode);

		/// <summary>
		/// トルクの適用
		/// </summary>
		/// <param name="torque"></param>
		/// <param name="mode"></param>
		void AddTorque(const Vector3& torque, ForceMode mode);

		/// <summary>
		/// 力の適用
		/// </summary>
		/// <param name="force"></param>
		/// <param name="point"></param>
		void AddForce(const Vector3& force, const Vector3& point);

		/// <summary>
		/// 衝突力の適用
		/// </summary>
		/// <param name="impulse"></param>
		/// <param name="pos"></param>
		void AddImpulse(const Vector3& impulse, const Vector3& pos);

		/// <summary>
		/// リセット
		/// </summary>
		void Reset();

		/// <summary>
		/// 衝突
		/// </summary>
		/// <param name="body"></param>
		void OnCollision(Body* body);

		// 衝突時に呼ばれる固有の処理
		/*virtual void OnCollisionEvent(Body* body = nullptr) = 0;
		virtual void OnTriggerEvent(Body* body = nullptr) = 0;*/

		/// <summary>
		/// getter
		/// </summary>
		/// <returns></returns>

		World* GetWorld() { return world_; }
		Vector3 GetVelocity() { return velocity_; }
		Vector3 GetAcceleration() { return acceleration_; }
		Vector3 GetAngularVelocity() { return angularVelocity_; }
		WorldTransform* GetWorldTransform() { return worldTransform_; };
		Vector3 GetTranslation() { return worldTransform_->translation_; }
		Vector3 GetMatWorldTranslation() { return { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] }; }
		Vector3 GetVertical() { return vertical_; }
		float GetMass() { return mass_; }
		float GetInverseMass() { return 1.0f / mass_; }
		float GetInertiaMoment() { return inertiaMoment_; }
		float GetDrag() { return drag_; }
		float GetAngularDrag() { return angularDrag_; }
		bool GetIsTrigger() { return isTrigger_; }
		float GetMiu() { return miu_; }
		float GetBounciness() { return bounciness_; }
		Vector3 GetNormalVector() { return normalVector_; }
		Matrix3x3 GetInertiaTensor() { return inertiaTensor_; };
		std::array<bool, 3> GetFixedMove() { return fixedMove_; }
		bool GetFixedMove(int axis) { return fixedMove_[axis]; }

		/// <summary>
		/// setter
		/// </summary>
		/// <param name="mass"></param>

		void SetMass(float mass) { mass_ = mass; }
		void SetInertiaMoment(float inertiaMoment) { inertiaMoment_ = inertiaMoment; }
		void SetDrag(float drag) { drag_ = drag; }
		void SetAngularDrag(float angularDrag) { angularDrag_ = angularDrag; }
		void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
		void SetGravity(const Vector3& gravity) { gravityAcceleration_ = gravity; }
		void SetAngularVelocity(const Vector3& angularVelocity) { angularVelocity_ = angularVelocity; }
		void SetIsTrigger(bool isTrigger) { isTrigger_ = isTrigger; }
		void SetMiu(float miu) { miu_ = miu; }
		void SetFirictionCombine(FrictionCombine frictionCombine) { frictionCombine_ = frictionCombine; }
		void SetBounciness(float bounciness) { bounciness_ = bounciness; }
		void SetBounceCombine(BounceCombine bounceCombine) { bounceCombine_ = bounceCombine; }
		void SetTranslation(Vector3 translation) { worldTransform_->translation_ = translation; }
		void SetMatTranslation(Vector3 translation) { worldTransform_->matWorld_.m[3][0] = translation.x, worldTransform_->matWorld_.m[3][1] = translation.y, worldTransform_->matWorld_.m[3][2] = translation.z; }
		void SetFixedMove(std::array<bool, 3> fixedMove) { fixedMove_ = fixedMove; }
		void SetFixedMove(int axis, bool fixedMove) { fixedMove_[axis] = fixedMove; }

	private:

		World* world_;
		WorldTransform* worldTransform_;
		// 速度
		Vector3 velocity_;
		// 角速度
		Vector3 angularVelocity_;
		// 加速度
		Vector3 acceleration_;
		Vector3 angularAcceleration_;
		// 力
		Vector3 force_;
		Vector3 torque_;
		// 質量
		float mass_;
		// 慣性モーメント
		float inertiaMoment_;
		// 慣性テンソル
		Matrix3x3 inertiaTensor_;
		// 空気抵抗
		float drag_ = 0.0005f;
		float angularDrag_ = 0.005f;
		// 重力加速度
		Vector3 gravityAcceleration_;
		// 摩擦計算方法
		FrictionCombine frictionCombine_;
		// 摩擦係数
		float miu_ = 0.0f;
		// 動摩擦力の大きさ
		float magnitude_ = 0.0f;
		// 反発計算方法
		BounceCombine bounceCombine_;
		// 反発係数
		float bounciness_ = 0.0f;
		// 法線
		Vector3 normalVector_;
		// 押し返し
		Vector3 pushback_;
		// 垂直抗力
		Vector3 vertical_;
		std::vector<PersistentManifold> persistentManifold_;
		float restitution_;
		bool isTrigger_;

		// 平行移動制限
		std::array<bool, 3> fixedMove_ = { false, false, false };
	};
}