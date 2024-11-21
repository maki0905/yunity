#pragma once

#include <list>
#include <memory>
#include<chrono>

#include "CollisionManager.h"
#include "MathFunction.h"


namespace yunity {
	class CollisionManager;
	class Joint;
	class Object3D;
}


namespace yunity{
	/*
	* @brief 物理空間クラス
	*/
	class World {
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="gravity"></param>
		void Initialize(const Vector3& gravity = { 0.0f, 0.0f, 0.0f });

		/// <summary>
		/// 解決
		/// </summary>
		void Solve();

		/// <summary>
		/// オブジェクト追加
		/// </summary>
		/// <param name="collider"></param>
		void Add(Object3D* collider) { allocator_.push_back(collider); };

		/// <summary>
		/// オブジェクト削除
		/// </summary>
		/// <param name="collider"></param>
		void Take(Object3D* collider);

		/// <summary>
		/// ジョイント追加
		/// </summary>
		/// <param name="joint"></param>
		void AddJoint(Joint* joint);

		/// <summary>
		/// ジョイント削除
		/// </summary>
		/// <param name="joint"></param>
		void TakeJoint(Joint* joint);

		/// <summary>
		/// アロケータ取得
		/// </summary>
		/// <returns></returns>
		std::list<Object3D*> GetAllocator() { return allocator_; }

		/// <summary>
		/// 重力設定
		/// </summary>
		/// <param name="gravity"></param>
		void SetGravity(const Vector3& gravity) { gravity_ = gravity; };

		/// <summary>
		/// 重力取得
		/// </summary>
		/// <returns></returns>
		Vector3 GetGravity() { return gravity_; };

		/// <summary>
		/// 固定タイム取得
		/// </summary>
		/// <param name="isFixedTime"></param>
		void SetFixedTime(bool isFixedTime) { isFixedTime_ = isFixedTime; }

	private:
		// デルタタイム
		float deltaTime_;
		const float fixedDeltaTime_ = 1.0f / 60.0f;

		std::chrono::steady_clock::time_point lastTime_;
		std::unique_ptr<CollisionManager> collisionManager_;
		std::list<Object3D*> allocator_;
		std::list<Joint*> jointAllocator_;
		Vector3 gravity_;
		bool isFixedTime_;

	};
}