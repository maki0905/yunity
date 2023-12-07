#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class SwapChain
{
public:
	SwapChain(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory);

	void Create();

	IDXGISwapChain4* GetSwapChain() {
		return swapChain_.Get();
	}

private:

	void CreateFactory();
	void CreateSwapChain();


private:

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;

};

