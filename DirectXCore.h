#pragma once

#include <d3d12.h>
#include <vector>

#include "Input.h"
#include "Device.h"
#include "WindowsAPI.h"
#include "DescriptorHeap.h"
#include "DescriptorHandle.h"

class SwapChain;
class CommandList;
class CommandQueue;
class DepthBuffer;
class BackBuffer;


class DirectXCore
{
public:
	enum class HeapType {
		kRTV,
		kSRV,
		kDSV
	};

public:
	static DirectXCore* GetInstance();

	void Initialize();

	void PreDraw();
	void PostDraw();

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
	
	//std::vector<DescriptorHeap*> descriptorHeaps_;
	//std::vector<DescriptorHandle*> descriptorHandles_;



};

