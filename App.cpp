#include "App.h"
#include "WindowsAPI.h"

App::App() {
	windowsAPI_ = WindowsAPI::GetInstance();
	windowsAPI_->CreateGameWindow();
}

App::~App() {
	// ゲームウィンドウの破棄
	windowsAPI_->TerminateGameWindow();
}

void App::Run()
{
	while (true) {
		if (windowsAPI_->ProcessMessage()) {
			break;
		}

	}
}
