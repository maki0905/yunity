#pragma once
#include "MathFunction.h"
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "Collider.h"

// 自機クラスの前方宣言
class Player;

class EnemyBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Camera* camera, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw();

	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	void SetPlayer(Player* player) { player_ = player; }

private:
	Camera* camera_;
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

	Player* player_;
};
