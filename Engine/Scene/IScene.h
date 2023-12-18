#pragma once

#include "Input.h"

enum SCENE { TITLE, GAME_STAGE, CLEAR, COUNT };

class IScene
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void DrawBack() = 0;
	virtual void Draw3D() = 0;
	virtual void DrawFront() = 0;

	virtual ~IScene();

	int GetSceneNo();

protected:
	static int sceneNo_;

};

