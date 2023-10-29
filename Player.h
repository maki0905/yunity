#pragma once

#include "BaseObject.h"
#include "Input.h"
#include <vector>


class Player : public BaseObject
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

	void Reset();

	void OnCollision(uint32_t collisionAttribute)override;
	void OnCollision(WorldTransform* worldTransform) override;

	bool GetIsAlive() { return isAlive_; }
	bool GetIsMove() { return isMove_; }
	Vector3 GetWorldPosition() { return Vector3(worldTransform_Body_.matWorld_.m[3][0], worldTransform_Body_.matWorld_.m[3][1], worldTransform_Body_.matWorld_.m[3][2]); };
	WorldTransform& GetWorldTransform() { return worldTransform_Body_; }
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void SetParent(WorldTransform* worldTrasnform) { worldTransform_Body_.parent_ = worldTrasnform; }

private:
	Input* input_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_Body_;
	WorldTransform worldTransform_Head_;
	WorldTransform worldTransform_Larm_;
	WorldTransform worldTransform_Rarm_;

	Model* model_ = nullptr;

	const float gravity_ = -1.0f;
	const float jumpValue_ = 6.0f;
	Vector3 velocity_;
	Vector3 acceleration_;
	bool isLanding_ = false;
	bool isMove_ = false;

	// 目標角度
	float destinationAngleY_ = 0.0f;

	// 
	bool isAlive_;
};

