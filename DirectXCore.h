#pragma once

#include <d3d12.h>
#include <memory>
#include <vector>

#include "Input.h"
#include "Device.h"


class SwapChain;
class CommandList;
class CommandQueue;
class DepthBuffer;
class BackBuffer;
class DescriptorHeap;
class DescriptorHandle;


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
	DescriptorHandle* GetDescriptorHandle(HeapType heapType) { return descriptorHandles_[static_cast<int>(heapType)].get(); }

private:
	DirectXCore() = default;
	~DirectXCore() = default;
	DirectXCore(const DirectXCore&) = delete;
	const DirectXCore& operator=(const DirectXCore&) = delete;
private:

	Device* device_ = nullptr;
	
	CommandList* commandList_ = nullptr;
	CommandQueue* commandQueue_ = nullptr;
	SwapChain* swapChin_ = nullptr;
	BackBuffer* backBuffer_ = nullptr;
	DepthBuffer* depthBuffer_ = nullptr;
	
	std::unique_ptr<DescriptorHeap> descriptorHeaps_[static_cast<int>(HeapType::kCount)];
	std::unique_ptr<DescriptorHandle> descriptorHandles_[static_cast<int>(HeapType::kCount)];
};

