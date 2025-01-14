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
#include "DirectionLight.h"

/*
* @brief ステージシーンクラス
*/
class StageScene : public yunity::IScene
{
	struct EndConstant {
		const Vector3 pos = { 1145.0f, 60.0f, 10.0f };
		const Vector3 scale = { 1.5f, 1.5f, 1.5f };
	};

	struct DirectionLight {
		const Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const Vector3 direction = { 1.0f, -1.0f, 0.0f };
		const Vector3 eyePosition = { 0.0f, 160.0f, 0.0f };
		const Vector3 targetPosition = { 0.0f, 0.0f, 1.0f };
		const Vector3 upDirection = { 0.0f, 1.0f, 0.0f };
		const float intensity = 1.0f;
		const float viewWidth = 2324;
		const float viewHight = 142.0f;
		const float nearClip = 0.0f;
		const float farClip = 600.0f;
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

	// ライト
	std::unique_ptr<yunity::DirectionLight> directionLight_;

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
};

