#pragma once
#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

/*
* @brief 浮遊パーティクル
*/
class FloatingParticle : public yunity::BaseParticle {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera"></param>
	void Initialize(yunity::Camera* camera) override;

	/// <summary>
	/// スポーン
	/// </summary>
	/// <param name="position"></param>
	void Spawn(const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ターゲット設定
	/// </summary>
	/// <param name="position"></param>
	void SetTarget(const Vector3 position) { targetPosition_ = position; isLerp_ = true; }

	/// <summary>
	/// 外部ファイルからデータ読み込み
	/// </summary>
	void ApplyGlobalVariables();
private:
	Random::RandomNumberGenerator rng;
	Vector3 targetPosition_;
	Vector3 centerPosition_;
	// 移動量
	float power_;
	// 生存時間
	float lifeTime_;
	// 角速度
	float speedDegree_;
	// 角度
	float angle_;
	// 周波数
	float frequencyTime_;
	// 
	bool isLerp_;
	Vector3 minPosition_ = {-2.0f, -2.0f, 0.0f};
	Vector3 maxPosition_ = {2.0f, 2.0f, 0.0f};

	// スポーン数
	uint32_t spawnCount_;

	// 指数
	float index_;
};