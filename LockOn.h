#pragma once

#include <memory>
#include <list>
#include "Sprite.h"
#include "ViewProjection.h"
#include "Input.h"
#include "MathFunction.h"


class Enemy;

/// <summary>
/// ロックオン
/// </summary>
class LockOn
{
public:
	enum class LockOnMode {
		kAuto,
		kManual,
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	void ReSearch(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	bool IsOutOfRange(const ViewProjection& viewProjection);

	Vector3 GetTargetPosition() const;

	bool ExistTarget()const { return target_ ? true : false; }
private:
	XINPUT_STATE joyStatePre_{};
	XINPUT_STATE joyState_{};

	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;
	uint32_t textureHandle_ = 0;

	//Vector2 sprite2DReticle_;

	// ロックオン対象
	const Enemy* target_ = nullptr;

	LockOnMode mode_ = LockOnMode::kAuto;

	float kDegrreToRadian = ConstParameter::PI / 180.0f;

	// 最小距離
	float minDistace_ = 10.0f;
	// 最大距離
	float maxDistance_ = 100.0f;
	// 角度範囲
	float angleRange_ = 20.0f * kDegrreToRadian;
};

