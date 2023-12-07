#pragma once
#include "Device.h"

class WindowsAPI;
class GraphicsEngine;
class Divece;
class T;
class Shader;
class SwapChain;
class CommandList;
class CommandQueue;
class DepthBuffer;
class BackBuffer;

class App
{
public:
	App();
	~App();
	void Run();

	void PreDraw();
	void PostDraw();

private:
	Device* device_ = nullptr;
	WindowsAPI* windowsAPI_ = nullptr;
	GraphicsEngine* graphicsEngine_ = nullptr;
	T* t_ = nullptr;
	Shader* shader_ = nullptr;

	CommandList* commandList_ = nullptr;
	CommandQueue* commandQueue_ = nullptr;

	UINT windowWidth_;
	UINT windowHeight_;

	SwapChain* swapChain_;
	BackBuffer* backBuffer_;
	DepthBuffer* depthBuffer_;
};

