#include "IScene.h"

int IScene::sceneNo_ = GAME_STAGE;

IScene::~IScene()
{
}

int IScene::GetSceneNo()
{
	return sceneNo_;
}
