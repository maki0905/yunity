#pragma once

#include <memory>

#include "IScene.h"

#include <memory>

#include "Camera.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Sprite.h"
#include "Skydome.h"
#include "Input.h"

#include "Sprite.h"
#include "Model.h"
class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;

private:
	XINPUT_STATE joyState_;
	XINPUT_STATE preJoyState_;
	Camera camera_;
	WorldTransform titleTransform_;

	std::unique_ptr<Model> title_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Sprite> pushA_;

	float fishing_;
	float rotationSpeed_;

	uint32_t time_;

	bool display_;
	bool flag_;

	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Model> model_ = nullptr;
};

