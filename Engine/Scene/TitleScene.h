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
	/*std::unique_ptr<Camera>*/ Camera* camera_;

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
	//std::unique_ptr<Model> model_ = nullptr;
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

	/*std::unique_ptr<Object3D>floor_;
	std::unique_ptr<Object3D>anchor_;
	std::unique_ptr<PrimitiveDrawer> sp_;
	std::unique_ptr<SpringJoint> joint_;*/

	// obj1,2
	std::unique_ptr<Object3D> obj1_;
	/*std::unique_ptr<Object3D> obj1_;
	std::unique_ptr<Object3D> obj2_;*/

	// springbrige
	/*std::unique_ptr<Object3D> objs_[5];
	std::unique_ptr<PrimitiveDrawer> lines_[4];
	std::unique_ptr<SpringJoint> springJoints_[4];
	float stiffness_ = 10.0f;
	float dampar_ = 0.1f;*/

	// pulley
	/*std::unique_ptr<Object3D> pulleyObjs_[2];
	std::unique_ptr<PrimitiveDrawer> pulleyLines_[3];
	std::unique_ptr<PulleyJoint> pulleyJoint_;
	Vector3 groundAnchor_[2];
	float ratio_;*/

	std::unique_ptr<ObjectManager> objectManager_;
};

