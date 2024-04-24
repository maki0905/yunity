#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <dxgi1_6.h>

class DescriptorHeap;

class BackBuffer
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device"></param>
	/// <param name="commandList"></param>
	/// <param name="swapChain"></param>
	BackBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain);

	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	/// <summary>
	/// レンダーターゲット設定
	/// </summary>
	/// <param name="swapChain"></param>
	/// <param name="dsvHeap_"></param>
	void SetRenderTarget(IDXGISwapChain4* swapChain, ID3D12DescriptorHeap* dsvHeap_);
	
	/// <summary>
	/// レンダーターゲットクリア
	/// </summary>
	/// <param name="swapChain"></param>
	void ClearRenderTarget(IDXGISwapChain4* swapChain);

	UINT GetBackBufferCount() { return backBufferCount_; }

	D3D12_CPU_DESCRIPTOR_HANDLE* GetCpuDescHandleRTV() { return rtvHandles_; }

private:
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	DescriptorHeap* rtvDescriptorHeap_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	UINT backBufferCount_ = 0;

};

