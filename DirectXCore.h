#pragma once

#include <d3d12.h>
#include <memory>
#include <vector>
#include <chrono>

#include "Input.h"
#include "Device.h"
#include "Shader.h"
#include "CommandList.h"
#include "ImGuiManager.h"
#include "BackBuffer.h"


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

	void PreDraw();
	void PostDraw();

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

	Device* device_ = nullptr;
	
	CommandList* commandList_ = nullptr;
	CommandQueue* commandQueue_ = nullptr;
	SwapChain* swapChain_ = nullptr;
	BackBuffer* backBuffer_ = nullptr;
	DepthBuffer* depthBuffer_ = nullptr;
	Shader* shader_ = nullptr;

	UINT windowWidth_;
	UINT windowHeight_;

	std::unique_ptr<DescriptorHeap> descriptorHeaps_[static_cast<int>(HeapType::kCount)];

	std::chrono::steady_clock::time_point reference_;

	ImGuiManager* imguiManager_ = nullptr;
	
};

