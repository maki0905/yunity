#pragma once

#include <memory>

#include "IScene.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Model.h"
#include "Sprite.h"
class StageScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;
private:
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Model> skydome_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector2 position_;

	Camera camera_;
	WorldTransform worldTransform_;
	
};

