#pragma once

#include <memory>

#include "IScene.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Model.h"
#include "World.h"
#include "Player.h"
#include "Skydome.h"
#include "Cloth.h"
#include "ObjectManager.h"
#include "SmokeParticle.h"
#include "FireworksParticle.h"
#include "PointParticle.h"


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
	yunity::Camera* camera_;

	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	yunity::WorldTransform worldTransform_;
	std::unique_ptr<yunity::Sprite> bottonSprite_ = nullptr;
	std::unique_ptr<yunity::Sprite> bottonPushSprite_ = nullptr;
	uint32_t time_;
	bool isDraw_;
	std::unique_ptr<yunity::Model> model_ = nullptr;
	std::unique_ptr<yunity::World> world_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<yunity::Object3D> obj_;
	std::unique_ptr<yunity::Skydome> skydome_;
	float amplitude = 0.5f;
	float wavelength = 0.0f;
	bool isStart_ = false;
	std::unique_ptr<yunity::Model> models_[3];
	yunity::WorldTransform TVworldTransform_[3];
	bool isActiveTV_[3];
	int preNum_;
	Easing grow_[3];
	Easing shrink_[3];
	uint32_t textureTV_[3];
	bool isMoveCamera[2];
	float moveCameraTimer_;
	Vector3 oldCameraPos_;
	std::unique_ptr<Cloth> cloth_;

	std::unique_ptr<ObjectManager> objectManager_;
};

