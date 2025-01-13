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
	/// getter
	/// </summary>

	bool GetActive() { return isActive_; }
	bool GetSelect() { return isSelect_; }
	Vector3 GetSpawnPoint() { return spawnPoint_; }

	/// <summary>
	/// setter
	/// </summary>

	void SetSelect(bool isSelect) { isSelect_ = isSelect; }
	void SetDisplayUI(bool flag, UI ui);
	void SetInGame(bool flag) { inGame_ = flag; }
	void SetSpawnPoint(const Vector3& spawnPoint) { spawnPoint_ = spawnPoint; }

private:
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	//std::unique_ptr<Model> model_;
	//WorldTransform worldTransform_;

	//Camera* camera_;

	bool inGame_;

	const float stiffness_ = 2.0f;
	const float dampar_ = 0.0f;
	float mass_;
	const float limitLength_ = 15.0f;

	bool isCrouching_;
	bool isHit_;
	bool isActive_;

	std::unique_ptr<yunity::PrimitiveDrawer> line_;
	Vector3 point_;
	bool isWire_;

	bool isJunp_;

	bool isFloot_;

	std::unique_ptr<yunity::Model> reticle3D_;
	//WorldTransform reticle3DWorldTransform_;
	std::unique_ptr<yunity::Model> apex_;
	yunity::WorldTransform apexWorldTransform_;


	std::unique_ptr<yunity::Sprite> reticle_;
	yunity::WorldTransform reticleWorldTransform_;
	uint32_t onReticle_;
	uint32_t offReticle_;

	bool isMoving_;

	bool isSelect_;

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

	std::unique_ptr<yunity::SpringJoint> springJoint_;
	std::unique_ptr<yunity::Body> apexBody_;

	std::unique_ptr<yunity::FixedJoint> fixedJoint_;
	std::unique_ptr<yunity::FixedJoint> playerFixedJoint_;

	std::unique_ptr<PointParticle> pointParticle_;
	std::unique_ptr<SmokeParticle> smokeParticle_;

	Random::RandomNumberGenerator rng;
	std::array<std::unique_ptr<FireworksParticle>, 10> fireworksParticles_;

	Vector3 spawnPoint_;

};

