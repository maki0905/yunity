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
#include "Object3D.h"

class DemoScene : public yunity::IScene
{
	enum DemoNumber {
		kDemo1,
		kDemo2,
		kDemo3,
		kDemo4,
		kDemo5,
		kDemo6,
		kDemo7,
		kDemo8,
		kDemo9,
		kDemo10,
		kMax
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
	void Demo1Initialize();
	void Demo2Initialize();
	void Demo3Initialize();
	void Demo4Initialize();
	void Demo5Initialize();
	void Demo1Update();
	void Demo2Update();
	void Demo3Update();
	void Demo4Update();
	void Demo5Update();

private:
	yunity::Camera* camera_;
	std::unique_ptr<yunity::DebugCamera> debugCamera_;

	DemoNumber demoNumber_;
	DemoNumber preDemoNumber_;


	std::unique_ptr<yunity::Object3D> object_;
	std::vector<std::unique_ptr<yunity::Object3D>> objects_;
	std::vector<std::unique_ptr<yunity::SpringJoint>> springJoints_;
	std::vector<std::unique_ptr<yunity::PrimitiveDrawer>> lines_;
	// 物理空間
	std::unique_ptr<yunity::World> world_;

	Vector3 force_;
	Vector3 velocity_;

	bool onoff_;
	bool isDebug_ = false;
};