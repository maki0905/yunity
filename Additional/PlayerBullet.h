#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include "MathFunction.h"
#include "Collider.h"

class Enemy;

/// <summary>
/// 自キャラの弾
/// </summary>

class PlayerBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera, const Vector3& positino, const Vector3 velocity);
	void Initialize(Camera* camera, const Vector3& position, const Vector3 velocity,Enemy* enemy);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	///<param name="viewProjection">ビュープロジェクション</param>
	void Draw();

	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// 速度
	Vector3 velocity_;
	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	Enemy* enemy_ = nullptr;
};
