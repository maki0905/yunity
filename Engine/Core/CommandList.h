#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "Vector4.h"

class BackBuffer;
class DepthBuffer;
class SwapChain;

class CommandList
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	CommandList(ID3D12Device* device);

	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	/// <summary>
	/// バリア変更
	/// </summary>
	/// <param name="swapChain">スワップチェーン</param>
	/// <param name="before">前</param>
	/// <param name="after">後</param>
	void BarrierChange(IDXGISwapChain4* swapChain, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	void BarrierChange(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	/// <summary>
	/// コマンドクリア
	/// </summary>
	void CommandClear();

	void OMSetRenderTargets(const D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescHandleRTV, ID3D12DescriptorHeap* dsvHeap = nullptr);
	void OMSetRenderTargets(const D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescHandleRTV, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);
	void ClearRenderTargetView(const Vector4 clearColorValue, const D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleRTV);
	void ClearDepthStencilView(ID3D12DescriptorHeap* dsvHeap);

	/// <summary>
	/// ビューポート設定
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void RSSetViewports(float width, float height);

	/// <summary>
	/// シザリング矩形
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void RSSetScissorRects(UINT width, UINT height);

	ID3D12GraphicsCommandList* GetCommandList() {
		return commandList_.Get();
	}

	ID3D12CommandAllocator* GetCommandAllocator() {
		return commandAllocator_.Get();
	}

private:
	/// <summary>
	/// コマンドアロケータ生成
	/// </summary>
	void CreateAllocator();

	/// <summary>
	/// コマンドリスト生成
	/// </summary>
	void CreateList();

private:

	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

};

