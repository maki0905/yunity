#pragma once

#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>



class GameScene
{
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw();

private:
	DirectXCommon* dxCommon_ = nullptr;

};

