#pragma once

#include <memory>

#include "Camera.h"
#include "Collider.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Object3D.h"
#include "Score/Score.h"

#include "SpringJoint.h"
#include "FixedJoint.h"

#include "Particle/SmokeParticle.h"
#include "Particle/FireworksParticle.h"

#include "Wire/Wire.h"
#include "GuideUI/GuideUI.h"
#include "PlayerProduction.h"
#include "Pose/Pose.h"


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
	/// 死亡時の初期化
	/// </summary>
	void InitializeDeth();

	/// <summary>
	/// 外部ファイルからデータ読み込み
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// 演出変更
	/// </summary>
	/// <param name="newState"></param>
	void ChangeProductionState(std::unique_ptr<PlayerProduction> newState);

	/// <summary>
	/// getter
	/// </summary>

	//bool GetActive() { return isActive_; }
	bool GetSelect() { return isSelect_; }
	bool GetIsPose() { return pose_->GetIsPose(); }
	Vector3 GetSpawnPoint() { return spawnPoint_; }
	Vector3 GetGoalPoint() { return goalPoint_; }
	Vector3 GetDeathPosition() { return deathPosition_; }
	Vector3 GetDeathCameraPosition() { return deathCameraPosition_; }
	Vector3 GetOldPlayerPosition() { return oldPlayerPosition_; }

	/// <summary>
	/// setter
	/// </summary>

	void SetSelect(bool isSelect) { isSelect_ = isSelect; }
	void SetDisplayUI(bool flag, UI ui);
	void SetSpawnPoint(const Vector3& spawnPoint) { spawnPoint_ = spawnPoint; }
	void SetIsScore(bool isScore) { isScore_ = isScore; }
	void SetIsReticle(bool isReticl) { isReticle_ = isReticl; }
	void SetDeathPosition(const Vector3& deathPosition) { deathPosition_ = deathPosition; }

private:
	// コントローラー
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	// しきい値
	float threshold_;

	// フラグ
	bool isMoving_;
	bool isSelect_;
	bool isCrouching_;
	bool isHit_;

	// プレイヤーのヒットボックスサイズ
	Vector3 hitBoxSize_;

	// スタート位置
	Vector3 startPosition_;

	// プレイヤーの物理パラメータ
	float mass_;
	float miu_;

	// ワイヤー
	std::unique_ptr<Wire> wire_;
	bool isReticle_;

	// ガイド
	std::unique_ptr<GuideUI> guideUI_;

	// スコア
	bool isScore_;
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

	// 固定ジョイント
	std::unique_ptr<yunity::FixedJoint> playerFixedJoint_;

	// 煙パーティクル
	std::unique_ptr<SmokeParticle> smokeParticle_;

	// 紙吹雪パーティクル
	static const int fireworksParticleQuantity_ = 10;
	std::array<std::unique_ptr<FireworksParticle>, fireworksParticleQuantity_> fireworksParticles_;

	// スタート位置
	Vector3 spawnPoint_;

	// ゴール位置
	Vector3 goalPoint_;

	// 演出用
	std::unique_ptr<PlayerProduction> productionState_;
	float time_;
	Vector3 deathPosition_;
	Vector3 deathCameraPosition_;
	Vector3 clearCameraPosition_;
	Vector3 oldPlayerPosition_;

	float deatLine_;
	// 動く床の許容範囲
	float toleranceLevel_;
	// コインの加算値
	int coinValue_;

	// ポーズ機能
	std::unique_ptr<Pose> pose_;

};

