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
* @brief クリアシーンクラス
*/
class ClearScene : public yunity::IScene
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
	yunity::Camera* camera_;
	std::unique_ptr<yunity::Object3D> object_;
	std::array<std::unique_ptr<yunity::Object3D>, 10> objects_;
	// 物理空間
	std::unique_ptr<yunity::World> world_;
	bool onoff_;
};

