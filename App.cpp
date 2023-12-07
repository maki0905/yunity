#include "App.h"
#include "WindowsAPI.h"
#include "GraphicsEngine.h"

App::App() {
	windowsAPI_ = WindowsAPI::GetInstance();
	windowsAPI_->CreateGameWindow();
	graphicsEngine_ = new GraphicsEngine();

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

		graphicsEngine_->PreDraw();

		graphicsEngine_->PostDraw();

	}
}
