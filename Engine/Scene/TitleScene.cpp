#include "TitleScene.h"

#include "SceneManager.h"

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
	}
}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
}

void TitleScene::DrawFront()
{
}

