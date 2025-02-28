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
		struct PersistentManifold {
			Object3D* colliderA;
			Object3D* colliderB;
			Vector3 localPointA;
			Vector3 localPointB;
			Vector3 contactNormal;
			float penetrationDepth;
		};

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

		void SolveConstraints();

		/// <summary>
		/// オブジェクト追加
		/// </summary>
		/// <param name="collider"></param>
		void AddObject(Object3D* collider) { objectList_.push_back(collider); };

		/// <summary>
		/// オブジェクト削除
		/// </summary>
		/// <param name="collider"></param>
		void TakeObject(Object3D* collider);

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

		void AddPersistentManifold(const PersistentManifold& persistentManifold) {
			PersistentManifolds_.push_back(persistentManifold);
		}

		/// <summary>
		/// アロケータ取得
		/// </summary>
		/// <returns></returns>
		std::list<Object3D*> GetAllocator() { return objectList_; }

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
		// 最後の時間
		std::chrono::steady_clock::time_point lastTime_;
		// 衝突マネージャー
		std::unique_ptr<CollisionManager> collisionManager_;
		// オブジェクトリスト
		std::list<Object3D*> objectList_;
		// ジョイントリスト
		std::list<Joint*> jointList_;
		// 重力
		Vector3 gravity_;
		// 固定時間フラグ
		bool isFixedTime_;
		std::vector<PersistentManifold> PersistentManifolds_;

	};
}