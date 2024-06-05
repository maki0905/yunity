#pragma once

#include <memory>

#include "IScene.h"
#include "CameraManager.h"
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
	/*std::unique_ptr<Camera>*/ Camera* camera_;

	WorldTransform worldTransform_;

	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Model> model_ = nullptr;
};

