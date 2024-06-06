#pragma once
#include <memory>

#include "IScene.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "Model.h"
#include "World.h"



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

	std::unique_ptr<Model> model0_;
	WorldTransform worldTransform0_;
	std::unique_ptr<Model> model1_;
	WorldTransform worldTransform1_;
	std::unique_ptr<Model> model2_;
	WorldTransform worldTransform2_;
};

