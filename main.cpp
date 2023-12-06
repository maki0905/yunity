#include <Windows.h>
#include "App.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	App* app = new App();
	app->Run();
	return 0;
}