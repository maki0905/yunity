#pragma once

#include <memory>

#include "IScene.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Model.h"
#include "World.h"
#include "Player.h"
#include "Skydome.h"

class SelectScene : public IScene
{
	struct Easing {
		bool flag;
		float t;
		Vector3 scale;
	};
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;

private:
	/*std::unique_ptr<Camera>*/ Camera* camera_;

	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	WorldTransform worldTransform_[3];
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Model> models_[3];
	bool isActiveTV_[3];
	uint32_t preNum_;
	Easing grow_[3];
	Easing shrink_[3];
	uint32_t textureTV_[3];
	std::unique_ptr<World> world_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Skydome> skydome_;
};