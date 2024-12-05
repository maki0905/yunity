#include <Windows.h>
#include "GameManager.h"
#include "RenderTexture.h"
#include "ResourceLeakChecker.h"



static ResourceLeakChecker leakChecker;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    std::unique_ptr<yunity::Framework> game = std::make_unique<GameManager>();
    game->Run();
    yunity::RenderTexture::GetInstance()->Finalize();
    return 0;
}