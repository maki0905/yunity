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
#include "ObjectManager.h"
#include "Skydome.h"
#include "Coin.h"

class StageScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;
private:
	/*bool isDebug_ = false;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<DebugCamera> debugCamera_;


	std::unique_ptr<Player> player_;
	std::unique_ptr<BlockManager> blockManager_;

	std::unique_ptr<Model> start_;
	WorldTransform worldTransform_start_;
	std::unique_ptr<Model> end_;
	WorldTransform worldTransform_end_;

	std::unique_ptr<World> world_;
	std::unique_ptr<PrimitiveDrawer> primitiveDrawer_;*/
	bool isDebug_ = false;

	Camera* camera_;
	//std::unique_ptr<Camera> camera_;
	std::unique_ptr<DebugCamera> debugCamera_;

	//std::vector<Object3D*> objects_;

	std::unique_ptr<SkyBox> skybox_;
	WorldTransform skyboxWorldTransform_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Player> player_;

	std::unique_ptr<World> world_;

	/*std::unique_ptr<Model> start_;
	WorldTransform startWT_;
	std::unique_ptr<Model> end_;
	WorldTransform endWT_;*/
	WorldTransform startWT_;
	std::vector<Object3D*> floors_;
	std::vector<Object3D*> trampolines_;

	std::unique_ptr<Coin> coin_;

	std::string stageName_ = "stage4";

};

