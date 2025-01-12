#pragma once

#include <memory>

#include "IScene.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Model.h"
#include "World.h"
#include "Player.h"
#include "Skydome.h"
#include "ObjectManager.h"
#include "DirectionLight.h"

/*
* @brief タイトルクラス
*/
class TitleScene : public yunity::IScene
{
	struct Easing {
		bool flag;
		float t;
		Vector3 scale;
	};

	struct DirectionLight {
		const Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const Vector3 direction = { 1.0f, -1.0f, 0.0f };
		const float intensity = 1.0f;
	};

	struct TVConstant {
		const Vector3 translation = { 80.0f, 18.0f, 12.0f };
		const float addition = 30.0f;
		const float rotationX = -15.0f * DegToRad();
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
	const Vector3 cameraPos_ = { 0.0f, 10.0f, -50.0f };

	// コントローラー
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	// 物理空間
	std::unique_ptr<yunity::World> world_;
	const Vector3 gravity_ = { 0.0f, -30.0f, 0.0f };

	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objectManager_;

	// skydome
	std::unique_ptr<yunity::Skydome> skydome_;
	const Vector3 skydomeScale_ = { 5.0f, 5.0f, 5.0f };

	// ボタンスプライト
	std::unique_ptr<yunity::Sprite> bottonSprite_ = nullptr;
	std::unique_ptr<yunity::Sprite> bottonPushSprite_ = nullptr;
	const Vector2 spritePos_ = { 610.0f, 520.0f };
	bool isStart_ = false;
	bool isDraw_;
	uint32_t time_;
	const uint32_t limitTime_ = 30;

	// プレイヤー
	std::unique_ptr<Player> player_;
	const float targetPoint_ = 35.0f;
	const float playerMass_ = 2.0f;

	std::unique_ptr<yunity::DirectionLight> directionLight_;
	
	//int preNum_;
};

