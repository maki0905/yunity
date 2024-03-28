#pragma once

#include <memory>

#include "IScene.h"

#include "Camera.h"
#include "DebugCamera.h"
#include "Player.h"
#include "BlockManager.h"
#include "Block.h"

#include "World.h"



class StageScene : public IScene
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


	std::unique_ptr<Player> player_;
	std::unique_ptr<BlockManager> blockManager_;

	std::unique_ptr<Model> start_;
	WorldTransform worldTransform_start_;
	std::unique_ptr<Model> end_;
	WorldTransform worldTransform_end_;

	std::unique_ptr<World> world_;
	std::unique_ptr<PrimitiveDrawer> primitiveDrawer_;

};

