#include <Windows.h>
#include "GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager* gameManager_ = new GameManager;
	gameManager_->Run();
	return 0;
}