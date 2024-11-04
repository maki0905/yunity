#pragma once

#include <memory>

#include "Camera.h"
#include "Collider.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Score.h"
#include "SpringJoint.h"
#include "FixedJoint.h"

/*
* @brief プレイヤークラス
*/
class Player : public Object3D
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="world">所属する物理空間</param>
	void Initialize(Camera* camera, World* world);

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
	/// プレイヤーの位置変更
	/// </summary>
	/// <param name="pos">位置</param>
	void ResetPos(const Vector3& pos);

	/// <summary>
	/// getter
	/// </summary>

	bool GetActive() { return isActive_; }
	bool GetSelect() { return isSelect_; }

	/// <summary>
	/// setter
	/// </summary>

	void SetSelect(bool isSelect) { isSelect_ = isSelect; }

private:
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	//std::unique_ptr<Model> model_;
	//WorldTransform worldTransform_;

	//Camera* camera_;


	float stiffness_;
	float dampar_;
	float mass_;
	float limitLength_;
	
	bool isCrouching_;
	bool isHit_;
	bool isActive_;

	std::unique_ptr<PrimitiveDrawer> line_;
	Vector3 point_;
	bool isWire_;

	bool isJunp_;

	bool isFloot_;

	std::unique_ptr<Model> reticle3D_;
	//WorldTransform reticle3DWorldTransform_;
	std::unique_ptr<Model> apex_;
	WorldTransform apexWorldTransform_;


	std::unique_ptr<Sprite> reticle_;
	WorldTransform reticleWorldTransform_;
	uint32_t onReticle_;
	uint32_t offReticle_;

	bool isMoving_;
	Body* collisionBody_;

	bool isSelect_;

	// スコア
	std::unique_ptr<Score> scoreUI_;

	// 制限速度
	float limitSpeed_ = 20.0f;

	std::unique_ptr<SpringJoint> springJoint_;
	std::unique_ptr<Body> apexBody_;

	std::unique_ptr<FixedJoint> fixedJoint_;
};

