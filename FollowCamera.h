#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
#include "Input.h"

// GameSceneの前方宣言
class GameScene;

class LockOn;

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const ViewProjection& viewProjection);

	/// <summary>
	///　更新
	/// </summary>
	void Update();

	/// <summary>
	/// 　Get
	/// </summary>
	ViewProjection* GetViewProjection() { return &viewProjection_; }


	/// <summary>
	/// 　Set
	/// </summary>
	void SetTarget(const WorldTransform* target);

	// 追従対象からのオフセットを計算する
	Vector3 Offset() const;

	void Reset();

	// 調整項目の適応
	void ApplyGlobalVariables();

	void SetLockOn(const LockOn* lockOn) { 
		lockOn_ = lockOn; 
	}

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// ゲームシーン
	std::unique_ptr<GameScene> gameScene_;
	// 追従対象
	const WorldTransform* target_ = nullptr;
	// 入力
	Input* input_ = nullptr;
	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	// 目標角度
	float destinationAngleY_ = 0.0f;

	float delayAmount_;

	// ロックオン
	const LockOn* lockOn_ = nullptr;


};