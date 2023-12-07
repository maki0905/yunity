#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <dxgi1_6.h>

class BackBuffer
{
public:
	BackBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain);

	void Create();

	void SetRenderTarget(IDXGISwapChain4* swapChain, ID3D12DescriptorHeap* dsvHeap_);
	void ClearRenderTarget(IDXGISwapChain4* swapChain);

private:
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
};

