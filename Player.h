#pragma once

#include "BaseObject.h"
#include "Input.h"
#include <vector>
#include <optional>


class Player : public BaseObject
{
public:
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kDash,   // ダッシュ中
	};

	struct WorkAttack {
		float parameter;
		float addValue;
		Vector3 start_Arm;
		Vector3 end_Arm;
		Vector3 start_Weapon;
		Vector3 end_Weapon;
	};

	struct WorkDash {
		float parameter;
		float time;
		float speed;
	};

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

	// 調整項目の適応
	void ApplyGlobalVariables();

	bool GetIsAlive() { return isAlive_; }
	bool GetIsMove() { return isMove_; }
	Behavior GetBehavior() { return behavior_; }
	Vector3 GetWorldPosition() { return Vector3(worldTransform_Body_.matWorld_.m[3][0], worldTransform_Body_.matWorld_.m[3][1], worldTransform_Body_.matWorld_.m[3][2]); };
	WorldTransform& GetWorldTransform() { return worldTransform_Body_; }
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void SetParent(WorldTransform* worldTrasnform) { worldTransform_Body_.parent_ = worldTrasnform; }

private:
	// 通常行動初期化
	void BehaviorRootInitialize();
	// 通常行動更新
	void BehaviorRootUpdate();
	// 攻撃行動初期化
	void BehaviorAttackInitialize();
	// 攻撃行動更新
	void BehaviorAttackUpdate();
	// ダッシュ行動初期化
	void BehaviorDashInitialize();
	// ダッシュ行動更新
	void BehaviorDashUpdate();


private:
	Input* input_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_Body_;
	WorldTransform worldTransform_Head_;
	WorldTransform worldTransform_Larm_;
	WorldTransform worldTransform_Rarm_;
	WorldTransform worldTransform_Weapon_;

	Model* model_ = nullptr;

	const float gravity_ = -0.1f;
	const float jumpValue_ = 1.0f;
	Vector3 velocity_;
	Vector3 acceleration_;
	bool isLanding_ = false;
	bool isMove_ = false;

	// 目標角度
	float destinationAngleY_ = 0.0f;

	// 
	bool isAlive_;

	// パラメータ
	WorkAttack workAttack_;
	WorkDash workDash_;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};

