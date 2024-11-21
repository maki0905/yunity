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
#include "FireworksParticle.h"

/*
* @brief タイトルクラス
*/
class TitleScene : public IScene
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
	Camera* camera_;

	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	WorldTransform worldTransform_;
	std::unique_ptr<Sprite> bottonSprite_ = nullptr;
	std::unique_ptr<Sprite> bottonPushSprite_ = nullptr;
	uint32_t time_;
	bool isDraw_;
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<World> world_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Object3D> obj_;
	std::unique_ptr<Skydome> skydome_;
	float amplitude = 0.5f;
	float wavelength = 0.0f;
	bool isStart_ = false;
	std::unique_ptr<Model> models_[3];
	WorldTransform TVworldTransform_[3];
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

	bool pflag_;
	std::unique_ptr<FireworksParticle> particle_;
};

