#pragma once

#include <d3d12.h>
#include <memory>
#include <vector>
#include <chrono>

#include "Input.h"
#include "Device.h"
#include "ShaderCompiler.h"
#include "CommandList.h"
#include "ImGuiManager.h"
#include "BackBuffer.h"
#include "RenderTexture.h"

#include "GraphicsPipelineManager.h"
#include "GraphicsCommon.h"

class SwapChain;
class CommandList;
class CommandQueue;
class DepthBuffer;
class BackBuffer;
class DescriptorHeap;

class DirectXCore
{
public:
	enum class HeapType {
		kRTV,
		kSRV,
		kDSV,
		kCount
	};

public:
	static DirectXCore* GetInstance();

	void Initialize();
	void Finalize();

	void PreDrawRenderTexture();
	void PostDrawRenderTexture();

	void PreDrawSwapchain();
	void PostDrawSwapchain();

	DescriptorHeap* GetDescriptorHeap(HeapType heapType) { return descriptorHeaps_[static_cast<int>(heapType)].get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_->GetCommandList(); }

	UINT GetBackBufferCount() { return backBuffer_->GetBackBufferCount(); }
	

private:
	void InitializeFixFPS();
	void UpdateFixFPS();

private:
	DirectXCore() = default;
	~DirectXCore() = default;
	DirectXCore(const DirectXCore&) = delete;
	const DirectXCore& operator=(const DirectXCore&) = delete;
private:
	std::unique_ptr<CommandList> commandList_;
	std::unique_ptr<CommandQueue> commandQueue_;
	std::unique_ptr<SwapChain> swapChain_;
	std::unique_ptr<BackBuffer> backBuffer_;
	std::unique_ptr<DepthBuffer> depthBuffer_;

	UINT windowWidth_;
	UINT windowHeight_;

	std::unique_ptr<DescriptorHeap> descriptorHeaps_[static_cast<int>(HeapType::kCount)];

	std::chrono::steady_clock::time_point reference_;
};

