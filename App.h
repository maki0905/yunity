#pragma once

class WindowsAPI;
class GraphicsEngine;


class App
{
public:
	App();
	~App();
	void Run();

private:
	WindowsAPI* windowsAPI_ = nullptr;
	GraphicsEngine* graphicsEngine_ = nullptr;
};

