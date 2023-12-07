#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class BackBuffer;
class DepthBuffer;
class SwapChain;

class CommandList
{
public:
	CommandList(ID3D12Device* device);

	void Create();

	void BarrierChange(IDXGISwapChain4* swapChain, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	void CommandClear();

	void SetViewport(float width, float height);

	void SetRect(UINT width, UINT height);

	ID3D12GraphicsCommandList* GetCommandList() {
		return commandList_.Get();
	}

	ID3D12CommandAllocator* GetCommandAllocator() {
		return commandAllocator_.Get();
	}

private:
	void CreateAllocator();

	void CreateList();

private:

	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

};

