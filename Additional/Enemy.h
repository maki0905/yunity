#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Input.h"
#include "MathFunction.h"
#include "ImGuiManager.h"
#include <list>
#include "directxmath.h"
#include "EnemyBullet.h"
#include "Collider.h"
#include "Sprite.h"


// 自機クラスの前方宣言
class Player;
// 敵クラスの前方宣言
class Enemy;

class TimedCall;

class StageScene;

class EnemyState {
public:
	virtual void Update(Enemy* pEnmy) = 0;


};

class EnemyPhaseApproach : public EnemyState {
public:
	void Update(Enemy* pEnemy);
};

class EnemyPhaseLeave : public EnemyState {
public:
	void Update(Enemy* pEnemy);
};


/// <summary>
/// 敵
/// </summary>
class Enemy : public Collider {
public:

	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();

	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Camera* camera, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="ViewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw();

	/// <summary>
	/// 接近フェーズの更新関数
	/// </summary>
	void ApproachPhase();

	/// <summary>
	/// 離脱フェーズの更新関数
	/// </summary>
	void LeavePhase();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void FireReset();

	// 発射間隔
	static const int kFireInterval = 60;

	// 接近フェーズ初期化
	void ApproachPhaseInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	bool IsDead() { return isDead_; }

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	void chageState(EnemyState* newState);

	void Move(const Vector3 move);

	Vector3 GetPosition() { return worldTransform_.translation_; }

	void TimedCallClear();

	Player* GetPlayer() { return player_; }

	void SetStageScene(StageScene* stageScene) { stageScene_ = stageScene; }

	void DrawLockon();

	bool IsLockon() { return isLockon_; }

private:
	// メンバ関数ポインタのテーブル
	static void (Enemy::* phaseFunc[])();
	EnemyState* state_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	std::list<EnemyBullet*> bullets_;
	// 発射タイマー
	int32_t fireTimer_ = 0;
	// 自キャラ
	Player* player_ = nullptr;
	// デスフラグ
	bool isDead_ = false;
	// 時限発動
	std::list<TimedCall*> timedCalls_;
	Camera* camera_;
	StageScene* stageScene_;
	bool isLockon_;
	std::unique_ptr<Sprite> sprite2DReticle_;
};


