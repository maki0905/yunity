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

	std::unique_ptr<Sprite> title_ = nullptr;
	std::unique_ptr<Sprite> pushA_ = nullptr;

	bool isDraw_;
	int timer_;

	XINPUT_STATE pad_;

};

