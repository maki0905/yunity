#pragma once

#include <memory>


#include "IScene.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "Model.h"
#include "World.h"
#include "SkyBox.h"
#include "Player/Player.h"
#include "ObjectManager/ObjectManager.h"
#include "Skydome.h"
#include "SpringBoard/SpringBoard.h"
#include "Particle/FireworksParticle.h"
#include "Random.h"
#include "DirectionLight.h"

/*
* @brief ステージシーンクラス
*/
class StageScene : public yunity::IScene
{
	struct DirectionLight {
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 direction = { 1.0f, -1.0f, 0.0f };
		Vector3 eyePosition = { 0.0f, 160.0f, 0.0f };
		Vector3 targetPosition = { 0.0f, 0.0f, 1.0f };
		Vector3 upDirection = { 0.0f, 1.0f, 0.0f };
		float intensity = 1.0f;
		float viewWidth = 2324;
		float viewHight = 142.0f;
		float nearClip = 0.0f;
		float farClip = 600.0f;
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
	Vector3 skydomeScale_ = { 10.0f, 10.0f, 10.0f };

	// プレイヤー
	Player* player_;

	// 物理空間
	Vector3 gravity_ = { 0.0f, -30.0f, 0.0f };
	std::unique_ptr<yunity::World> world_;

	// ライト
	std::unique_ptr<yunity::DirectionLight> directionLight_;

	// ステージ名
	std::string stageName_ = "stage4";

	// カメラワーク
	float moveCameraTimer_;
	Vector3 oldCameraPos_;

	// スタート
	Vector3 startPos_;

	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objectManager_;
};

