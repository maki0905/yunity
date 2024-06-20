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

class ClearScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;



private:
	bool isDebug_ = false;

	std::unique_ptr<Camera> camera_;
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

