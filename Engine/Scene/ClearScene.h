#pragma once
#include <memory>

#include "IScene.h"
#include "Object3D.h"
#include "World.h"
#include "CameraManager.h"

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

	// コントローラー
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	// 物理空間
	std::unique_ptr<yunity::World> world_;
	Vector3 gravity_ = {0.0f, -9.8f, 0.0f};

	std::unique_ptr<yunity::Object3D> floor_;

};

