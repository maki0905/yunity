#pragma once

#include <memory>


#include "IScene.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "Model.h"
#include "World.h"
#include "SkyBox.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Skydome.h"
#include "SpringBoard.h"
#include "FireworksParticle.h"
#include "Random.h"

/*
* @brief ステージシーンクラス
*/
class StageScene : public yunity::IScene
{
	struct EndConstant {
		const Vector3 pos = { 997.0f, 60.0f, 10.0f };
		const Vector3 scale = { 1.5f, 1.5f, 1.5f };
	};

	struct DirectionLight {
		const Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const Vector3 direction = { 1.0f, -1.0f, 0.0f };
		const float intensity = 1.0f;
	};

	struct StartConstant {
		const Vector3 translation = { 0.0f, 1.0f, 3.0f };
		const Vector3 scale = { 0.5f, 0.5f, 0.5f };
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawBack() override;

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw3D() override;

	/// <summary>
	/// 前景描画
	/// </summary>
	void DrawFront() override;
private:
	bool isDebug_ = false;
	bool inStage_ = false;

	yunity::Camera* camera_;
	std::unique_ptr<yunity::DebugCamera> debugCamera_;

	// skydome
	std::unique_ptr<yunity::Skydome> skydome_;

	// プレイヤー
	std::unique_ptr<Player> player_;

	// 物理空間
	const Vector3 gravity_ = { 0.0f, -30.0f, 0.0f };
	std::unique_ptr<yunity::World> world_;

	// ステージ名
	std::string stageName_ = "stage4";

	// カメラワーク
	float moveCameraTimer_;
	Vector3 oldCameraPos_;

	// スタート
	std::unique_ptr<yunity::Model> start_;
	yunity::WorldTransform startWT_;
	Vector3 startPos_;

	// ゴール
	std::unique_ptr<yunity::Model> end_;
	yunity::WorldTransform endWT_;
	EndConstant endConstant_;
	uint32_t textureTV_;

	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objectManager_;

	bool isReset_;
	bool isDebt_;
	float upTime_;
	float downTime_;
	float resetTime_;
	float sizeTime_;
	Vector3 dieCamera_;
	Vector3 diePos_;
	Vector3 topPos_;

	bool isGoal_;
	bool isScore_;
	bool isClear_;
	float time_;
	Vector3 playerPos_;
	Vector3 cameraPos_;

	// ジャンプ台
	static const uint32_t springBoardCount_ = 2;
	std::array<std::unique_ptr<SpringBoard>, springBoardCount_> springTops_;
	std::array<std::unique_ptr<yunity::Object3D>, springBoardCount_> springAnchors_;
	std::array<std::unique_ptr<yunity::PrimitiveDrawer>, springBoardCount_> springLines_;

	// 吊り橋
	static const uint32_t bridgeCount_ = 6;
	std::array<std::unique_ptr<yunity::Object3D>, bridgeCount_> bridge_;
	std::array<std::unique_ptr<yunity::PrimitiveDrawer>, bridgeCount_ - 1> bridgeLines_;
	std::array<std::unique_ptr<yunity::SpringJoint>, bridgeCount_ - 1> bridgesJoint_;
	const float stiffness_ = 5.0f;
	const float damper_ = 1.0f;

	static const uint32_t bridge2Count_ = 16;
	std::array<std::unique_ptr<yunity::Object3D>, bridge2Count_> bridge2_;
	std::array<std::unique_ptr<yunity::PrimitiveDrawer>, bridge2Count_ - 1> bridge2Lines_;
	std::array<std::unique_ptr<yunity::SpringJoint>, bridge2Count_ - 1> bridges2Joint_;
	const float stiffness2_ = 10.0f;
	const float damper2_ = 1.0f;
};

