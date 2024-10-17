#pragma once
#include <memory>

#include "IScene.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "Model.h"
#include "World.h"
#include "SkyBox.h"
#include "Player.h"
#include "World.h"
#include "Object3D.h"

/*
* @brief クリアシーンクラス
*/
class ClearScene : public IScene
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
	bool isDebug_ = false;

	Camera* camera_;
	std::unique_ptr<DebugCamera> debugCamera_;

	std::vector<Object3D*> objects_;

	std::unique_ptr<Model> model0_;
	WorldTransform worldTransform0_;
	std::unique_ptr<Model> model1_;
	WorldTransform worldTransform1_;
	std::unique_ptr<Model> model2_;
	WorldTransform worldTransform2_;
	std::unique_ptr<SkyBox> skybox_;
	std::unique_ptr<Player> player_;

	std::unique_ptr<World> world_;

};

