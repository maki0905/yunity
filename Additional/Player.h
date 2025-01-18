#pragma once

#include <memory>

#include "Camera.h"
#include "Collider.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Object3D.h"
#include "Score.h"
#include "SpringJoint.h"
#include "FixedJoint.h"

#include "PointParticle.h"
#include "SmokeParticle.h"
#include "FireworksParticle.h"

namespace yunity {
	class Sprite;
}

/*
* @brief プレイヤークラス
*/
class Player : public yunity::Object3D
{
public:
	enum class UI {
		kScore,
		kReticle
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="world">所属する物理空間</param>
	void Initialize(yunity::Camera* camera, yunity::World* world);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 衝突時のイベント
	/// </summary>
	void OnCollisionEvent() override;

	/// <summary>
	/// 衝突時のイベント
	/// </summary>
	void OnTriggerEvent() override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset(const Vector3& pos);

	/// <summary>
	/// リザルト
	/// </summary>
	bool Result();

	/// <summary>
	/// ステージ入室時演出
	/// </summary>
	void InGameProduction();

	/// <summary>
	/// リセット時演出
	/// </summary>
	void ResetProduction();

	/// <summary>
	/// 死亡時演出
	/// </summary>
	void DeathProduction();

	/// <summary>
	/// ゴール時演出
	/// </summary>
	void GoalProduction();

	/// <summary>
	/// 死亡時の初期化
	/// </summary>
	void InitializeDeth();

	/// <summary>
	/// getter
	/// </summary>

	bool GetActive() { return isActive_; }
	bool GetSelect() { return isSelect_; }
	Vector3 GetSpawnPoint() { return spawnPoint_; }
	Vector3 GetGoalPoint() { return goalPoint_; }

	/// <summary>
	/// setter
	/// </summary>

	void SetSelect(bool isSelect) { isSelect_ = isSelect; }
	void SetDisplayUI(bool flag, UI ui);
	void SetSpawnPoint(const Vector3& spawnPoint) { spawnPoint_ = spawnPoint; }

private:
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	bool isWire_;
	bool isJump_;
	bool isHitRay_;
	bool isMoving_;
	bool isSelect_;
	bool isCrouching_;
	bool isHit_;
	bool isActive_;

	const float stiffness_ = 2.0f;
	const float dampar_ = 0.1f;
	const float limitLength_ = 20.0f;

	// ワイヤー描画用
	std::unique_ptr<yunity::PrimitiveDrawer> line_;
	Vector3 point_;

	// 3Dレティクル
	std::unique_ptr<yunity::Model> reticle3D_;
	std::unique_ptr<yunity::Model> apex_;
	yunity::WorldTransform apexWorldTransform_;

	// 2Dレティクル
	std::unique_ptr<yunity::Sprite> reticle_;
	yunity::WorldTransform reticleWorldTransform_;
	std::unique_ptr<yunity::Sprite> landingPoint_;
	yunity::WorldTransform landingPointWorldTrans_;
	uint32_t onReticle_;
	uint32_t offReticle_;

	// スコア
	bool isScore_;
	bool isReticle_;
	std::unique_ptr<Score> scoreUI_;
	float lerpTime_;
	float displayTime_;

	// 速度
	const float fixedSpeed_ = 2.0f;
	const float floatSpeed_ = 0.1f;

	// 制限速度
	const float limitSpeed_ = 20.0f;

	// スプリングジョイント
	std::unique_ptr<yunity::SpringJoint> springJoint_;

	// 固定ジョイント
	std::unique_ptr<yunity::FixedJoint> fixedJoint_;
	std::unique_ptr<yunity::FixedJoint> playerFixedJoint_;

	// ワイヤーの先端のボディ
	std::unique_ptr<yunity::Body> apexBody_;

	// パーティクル
	std::unique_ptr<PointParticle> pointParticle_;
	std::unique_ptr<SmokeParticle> smokeParticle_;
	Random::RandomNumberGenerator rng;
	std::array<std::unique_ptr<FireworksParticle>, 10> fireworksParticles_;

	// スタート位置
	Vector3 spawnPoint_;

	// ゴール位置
	Vector3 goalPoint_;

	// 演出用
	float time_;
	Vector3 diePlayerPosition_;
	Vector3 dieCameraPosition_;
	Vector3 topPos_;
	Vector3 clearCameraPosition_;
	Vector3 oldPlayerPosition_;
	const Vector3 setCameraPos_ = { 0.0, 5.0f, -50.0f };
	const Vector3 dieUp_ = { 0.0f, 10.0f, -10.0f };
	const Vector3 dieDown_ = { 0.0f, -20.0f, -10.0f };
	const float zeemOutPositionZ = -60.0f;
	const float resetTime_ = 2.0f;
	const float dieUpTime_ = 4.0f;
	const float dieDownTime_ = 2.0f;
	const float goalTime_ = 0.5f;
	const float clearTime_ = 2.0f;

};

