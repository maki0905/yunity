#pragma once

#include <memory>

#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "PrimitiveDrawer.h"
#include "World.h"
#include "Body.h"
#include "SpringJoint.h"
#include "FixedJoint.h"
#include "PointParticle.h"

class Player;

class Wire {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="world"></param>
	/// <param name="worldTransform"></param>
	/// <param name="player"></param>
	void Initialize(yunity::Camera* camera, yunity::World* world, yunity::WorldTransform* worldTransform, Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw3D();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	void Reset();

	/// <summary>
	/// 外部データ読み込み
	/// </summary>
	void ApplyGlobalVariables();

	bool GetIsWire() { return isWire_; };
	void SetIsWire(bool isWire) { isWire_ = isWire; }
private:
	// コントローラー
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	bool isWire_;
	bool isHitRay_;

	// レティクルの移動速度
	float reticleSpeed_;

	// 物理空間
	yunity::World* world_;

	// カメラ
	yunity::Camera* camera_;

	// プレイヤーのワールドトランスフォーム
	yunity::WorldTransform* playerWorldTransform_;

	// ワイヤーの剛性
	float stiffness_;
	// ワイヤーの減衰
	float dampar_;

	// 3Dレティクル
	std::unique_ptr<yunity::Model> reticle3D_;
	std::unique_ptr<yunity::Model> apex_;
	yunity::WorldTransform apexWorldTransform_;

	// 2Dレティクル
	std::unique_ptr<yunity::Sprite> reticle_;
	yunity::WorldTransform reticleWorldTransform_;
	// 設置点レティクル
	std::unique_ptr<yunity::Sprite> landingPoint_;
	yunity::WorldTransform landingPointWorldTrans_;

	// ワイヤーの先端のボディ
	std::unique_ptr<yunity::Body> apexBody_;

	// スプリングジョイント
	std::unique_ptr<yunity::SpringJoint> springJoint_;

	// 固定ジョイント
	std::unique_ptr<yunity::FixedJoint> fixedJoint_;

	// ワイヤーの最大長さ
	float limitLength_;
	// ワイヤーのセグメントの長さ
	float segmentLength_;

	// ワイヤー描画用
	std::vector<std::unique_ptr<yunity::PrimitiveDrawer>> lines_;
	std::vector<Vector3> lineVertexs_;
	Vector4 lineColore_;
	// 線の描画の最大数
	int maxLines_;

	// 設置時パーティクル
	std::unique_ptr<PointParticle> pointParticle_;

};