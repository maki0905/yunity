#pragma once

#include <memory>


#include "IScene.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "Model.h"
#include "World.h"
#include "SkyBox.h"
#include "Player.h"
#include "World.h"
#include "ObjectManager.h"
#include "Skydome.h"
#include "Coin.h"
#include "SpringBoard.h"
#include "FireworksParticle.h"
#include "Random.h"

/*
* @brief ステージシーンクラス
*/
class StageScene : public yunity::IScene
{
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

	std::unique_ptr<yunity::SkyBox> skybox_;
	yunity::WorldTransform skyboxWorldTransform_;
	std::unique_ptr<yunity::Skydome> skydome_;
	std::unique_ptr<Player> player_;

	std::unique_ptr<yunity::World> world_;

	std::string stageName_ = "stage1";

	float moveCameraTimer_;
	Vector3 oldCameraPos_;

	std::unique_ptr<yunity::Model> start_;
	yunity::WorldTransform startWT_;
	std::unique_ptr<yunity::Model> end_;
	yunity::WorldTransform endWT_;
	Vector3 startPos_;
	Vector3 endPos_ = { 570.0f, 35.0f, 10.0f };
	uint32_t textureTV_;

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
	/*static const uint32_t bridgeCount_ = 9;
	std::array<std::unique_ptr<yunity::Object3D>, bridgeCount_> bridge_;
	std::array<std::unique_ptr<yunity::PrimitiveDrawer>, bridgeCount_ - 1> bridgeLines_;
	std::array<std::unique_ptr<yunity::SpringJoint>, bridgeCount_ - 1> bridgesJoint_;
	const float stiffness_ = 5.0f;
	const float damper_ = 0.1f;*/
};

