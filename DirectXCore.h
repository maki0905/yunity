#pragma once

#include <d3d12.h>
#include <memory>
#include <vector>

#include "Input.h"
#include "Device.h"
#include "Shader.h"
#include "CommandList.h"


class SwapChain;
class CommandList;
class CommandQueue;
class DepthBuffer;
class BackBuffer;
class DescriptorHeap;
class T;

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

	T* t_ = nullptr;
	
};

