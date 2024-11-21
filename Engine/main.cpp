#include <Windows.h>
#include "GameManager.h"
#include "RenderTexture.h"
#include "ResourceLeakChecker.h"



static ResourceLeakChecker leakChecker;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    yunity::Framework* game = new GameManager();
    game->Run();
    delete game;
    yunity::RenderTexture::GetInstance()->Finalize();
	return 0;
}