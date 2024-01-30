#pragma once

#include "Camera.h"
#include "WorldTransform.h"
#include <memory>
#include "Input.h"

class StageScene;
class LockOn;

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Camera& camera);

	/// <summary>
	///　更新
	/// </summary>
	void Update();

	/// <summary>
	/// 　Get
	/// </summary>
	Camera* GetViewProjection() { return &camera_; }


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
	Camera camera_;
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