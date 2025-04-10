#pragma once

#include <memory>

#include "IScene.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Model.h"
#include "World.h"
#include "Player/Player.h"
#include "Skydome.h"
#include "ObjectManager/ObjectManager.h"
#include "DirectionLight.h"

/*
* @brief タイトルクラス
*/
class TitleScene : public yunity::IScene
{
	struct DirectionLight {
		Vector4 color;
		Vector3 direction;
		Vector3 eyePosition;
		Vector3 targetPosition;
		Vector3 upDirection;
		float intensity;
		float viewWidth;
		float viewHight;
		float nearClip;
		float farClip;
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
	// カメラ
	yunity::Camera* camera_;
	Vector3 cameraPos_;

	// コントローラー
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	// 物理空間
	std::unique_ptr<yunity::World> world_;
	Vector3 gravity_;

	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objectManager_;

	// skydome
	std::unique_ptr<yunity::Skydome> skydome_;
	Vector3 skydomeScale_;

	// ボタンスプライト
	std::unique_ptr<yunity::Sprite> bottonSprite_ = nullptr;
	std::unique_ptr<yunity::Sprite> bottonPushSprite_ = nullptr;
	Vector2 spritePos_;
	bool isStart_ = false;
	bool isDraw_;
	float time_;
	float limitTime_;

	// プレイヤー
	Player* player_;
	float targetPoint_;
	float playerMass_;

	std::string stageName_ = "title";

	std::unique_ptr<yunity::DirectionLight> directionLight_;
};

