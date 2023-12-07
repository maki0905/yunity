#pragma once

#include <d3d12.h>
#include <wrl.h>

class Device;
class SwapChain;
class CommandList;
class CommandQueue;
class DepthBuffer;
class BackBuffer;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	void PreDraw();
	void PostDraw();

private:
	Device* device_ = nullptr;
	CommandList* commandList_ = nullptr;
	CommandQueue* commandQueue_ = nullptr;

	UINT windowWidth_;
	UINT windowHeight_;

	SwapChain* swapChain_;
	BackBuffer* backBuffer_;
	DepthBuffer* depthBuffer_;

};

