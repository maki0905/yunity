#pragma once

#include <memory>

#include "IScene.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Model.h"
#include "World.h"
#include "Player.h"
#include "Skydome.h"

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;

private:
	/*std::unique_ptr<Camera>*/ Camera* camera_;

	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	WorldTransform worldTransform_;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<World> world_;
	//std::unique_ptr<Player> player_;
	std::unique_ptr<Object3D> obj_;
	std::unique_ptr<Skydome> skydome_;
	float k = 0.5f;
	float i = 0.0f;

	std::unique_ptr<Object3D> obj1_;
	std::unique_ptr<Object3D> obj2_;
	std::unique_ptr<Object3D> obj3_;
	std::unique_ptr<Object3D> obj4_;
	std::unique_ptr<Object3D> obj5_;

	std::unique_ptr<Object3D> objs_[5];
	std::unique_ptr<PrimitiveDrawer> lines_[4];
	std::unique_ptr<SpringJoint> springJoints_[4];


	bool flag_ = true;

	Vector3 torupu_;

	float stiffness_ = 10.0f;
	float dampar_ = 0.1f;

	//std::unique_ptr<Model> model_ = nullptr;
};

