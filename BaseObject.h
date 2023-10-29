#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "CollisionConfig.h"

class BaseObject {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models_">モデルデータ配列</param>
	virtual void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	// 半径を取得
	virtual float GetRadius() { return radius_; }
	// 半径を設定
	virtual void SetRadius(float radius) { radius_ = radius; }
	// 衝突時に呼ばれる関数
	virtual void OnCollision(uint32_t collisionAttribute);
	virtual void OnCollision(WorldTransform* worldTransfrom);
	// ワールド座標を取得
	virtual Vector3 GetWorldPosition();

	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	// 衝突属性(自分)を取得
	uint32_t GetCollisionMask() { return CollisionMask_; }
	// 衝突属性(自分)を設定
	void SetCollisionMask(uint32_t collisionMask) { CollisionMask_ = collisionMask; }

	Sphere* GetSphere() { return &sphere_; }
	Plane* GetPlane() { return &plane_; }
	AABB* GetAABB() { return &aabb_; }
protected:
	// モデルデータ配列
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 衝突
	Sphere sphere_;
	Plane plane_;
	AABB aabb_;
	Vector3 size_;

private:
	float radius_ = 2.0f;
	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;

};
