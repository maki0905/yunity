#pragma once

#include "BaseObject.h"
#include <vector>

class Enemy : public BaseObject
{
public:
	struct WorkDestroy {
		uint32_t parameter;
		float angle;
	};
	const uint32_t effectTime = 60;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	void OnCollision(uint32_t collisionMask) override;

	// 移動
	void Move();

	Vector3 GetWorldPosition() { return Vector3(worldTransform_Body_.matWorld_.m[3][0], worldTransform_Body_.matWorld_.m[3][1], worldTransform_Body_.matWorld_.m[3][2]); };

	void Reset();

	void SetTranslation(const Vector3& translation);

	bool GetIsActive() const{ return isActive_; }

	bool GetIsDestroy() const { return isDestroy_; }

	Vector3 GetModelCenter() const ;


private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_Body_;
	WorldTransform worldTransform_Larm_;
	WorldTransform worldTransform_Rarm_;

	// 浮遊移動のサイクル<frame>
	const int32_t period = 60;
	// 浮遊の振幅<m>
	const float amplitude = 1.0f;

	float angle_;
	float angularVelocity_;
	float flightingParameter_;

	WorkDestroy workDestroy_;

	Vector3 movePre_;

	Vector3 startPos_;

	bool isDestroy_;
	bool isActive_;
	bool isHit_;

	uint32_t HP_ = 3;

	uint32_t coolTime_ = 20;

	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

};

