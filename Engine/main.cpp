#include <Windows.h>
#include "GameManager.h"
#include "RenderTexture.h"
#include "ResourceLeakChecker.h"



static ResourceLeakChecker leakChecker;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Framework* game = new GameManager();
    game->Run();
    delete game;
    RenderTexture::GetInstance()->Finalize();
	return 0;
}