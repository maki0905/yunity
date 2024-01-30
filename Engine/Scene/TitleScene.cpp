#include "TitleScene.h"
#include "Screen.h"

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Screen::GetInstance()->Playback(Screen::Type::Fade_Out_In, 2.0f, 2.0f);
	}
	if (Screen::GetInstance()->GetFlap()) {
		sceneNo_ = GAME_STAGE;
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

