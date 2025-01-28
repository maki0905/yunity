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
	// コントローラー
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	// しきい値
	float threshold_;
	float reticleSpeed_;

	// フラグ
	bool isWire_;
	bool isJump_;
	bool isHitRay_;
	bool isMoving_;
	bool isSelect_;
	bool isCrouching_;
	bool isHit_;
	bool isActive_;

	// プレイヤーのヒットボックスサイズ
	Vector3 hitBoxSize_;

	// スタート位置
	Vector3 startPosition_;

	// プレイヤーの物理パラメータ
	float mass_;
	float miu_;
	float stiffness_;
	float dampar_;

	// ワイヤーの最大長さ
	float limitLength_;
	// ワイヤーのセグメントの長さ
	float segmentLength_;

	// ワイヤー描画用
	std::vector<std::unique_ptr<yunity::PrimitiveDrawer>> lines_;
	std::vector<Vector3> lineVertexs_;
	std::unique_ptr<yunity::PrimitiveDrawer> line_;
	Vector3 point_;
	Vector4 lineColore_ ;
	int maxLines_;

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

	// ガイド
	std::unique_ptr<yunity::Sprite> guideA_;
	std::array<uint32_t, 2> guideATexture_;
	std::unique_ptr<yunity::Sprite> guideRB_;
	std::array<uint32_t, 2> guideRBTexture_;
	std::unique_ptr<yunity::Sprite> guideJump_;
	std::unique_ptr<yunity::Sprite> guideWire_;
	std::array<uint32_t, 2> guideWireTexture_;
	Vector2 guideAPosition_;
	Vector2 guideASize_;
	Vector2 guideRBPosition_;
	Vector2 guideRBSize_;
	Vector2 guideJumpPosition_;
	Vector2 guideJumpSize_;
	Vector2 guideWirePosition_;
	Vector2 guideWireSize_;

	// スコア
	bool isScore_;
	bool isReticle_;
	std::unique_ptr<Score> scoreUI_;
	float lerpTime_;
	float limitLerpTime_;
	float displayTime_;
	float limitDisplayeTime_ ;
	Vector2 scoreStartPosition_;
	Vector2 scoreEndPosition_;

	// 速度
	float fixedSpeed_;
	float floatSpeed_;

	// ジャンプ力
	Vector3 jumpPower_;

	// 制限速度
	float limitSpeed_;

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
	static const int fireworksParticleQuantity_ = 10;
	std::array<std::unique_ptr<FireworksParticle>, fireworksParticleQuantity_> fireworksParticles_;

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
	Vector3 setCameraPos_;
	Vector3 dieUp_;
	Vector3 dieDown_;
	Vector3 dieForce_;
    float zeemOutPositionZ_;
    float resetTime_;
    float dieUpTime_;
    float dieDownTime_;
    float goalTime_;
    float clearTime_;

	float deatLine_;

	float toleranceLevel_;
	int coinValue_;

};

