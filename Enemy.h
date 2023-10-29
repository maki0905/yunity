#pragma once

#include "BaseObject.h"
#include <vector>

class Enemy : public BaseObject
{
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

	// 移動
	void Move();

	Vector3 GetWorldPosition() { return Vector3(worldTransform_Body_.matWorld_.m[3][0], worldTransform_Body_.matWorld_.m[3][1], worldTransform_Body_.matWorld_.m[3][2]); };

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

};

