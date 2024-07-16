#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "CommandQueue.h"

class SwapChain
{
public:
	/// <summary>
	/// スワップチェーン生成
	/// </summary>
	void Create(ID3D12CommandQueue* commandQueue);

	IDXGISwapChain4* GetSwapChain() {
		return swapChain_.Get();
	}

private:

	/*Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;*/
	//std::unique_ptr<CommandQueue> commandQueue_;
	//ID3D12CommandQueue* commandQueue_;
	//IDXGIFactory7* dxgiFactory_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;

};

