#pragma once

#include "Input.h"

enum SCENE { TITLE, GAME_STAGE, CLEAR, COUNT };

class IScene
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~IScene();

	int GetSceneNo();

protected:
	static int sceneNo_;

};

