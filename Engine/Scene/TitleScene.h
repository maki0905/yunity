#pragma once

#include <memory>

#include "IScene.h"
#include "Sprite.h"
#include "Model.h"
#include "ParticleManager.h"
class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;

private:
	std::unique_ptr<Camera> camera_;

	WorldTransform worldTransform_;

	std::unique_ptr<Model> model0_ = nullptr;
	std::unique_ptr<Model> model1_ = nullptr;

	std::unique_ptr<ParticleManager> particleManager_ = nullptr;

};

