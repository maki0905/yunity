#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "DescriptorHeap.h"

class DescriptorHeap;

class DepthBuffer
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	DepthBuffer();

	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	/// <summary>
	/// デプスビュークリア
	/// </summary>
	void ClearDepthView();

	ID3D12DescriptorHeap* GetDescriptorHeap() { return dsvHeap_->GetHeapPointer(); }
	ID3D12Resource* GetDepthStencil() { return depthStencilResource_.Get(); }
	//D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() { return D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetHeapPointer()->GetCPUDescriptorHandleForHeapStart()); }


private:

	/// <summary>
	/// デプスステンシル生成
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource();

private:

	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;

	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;

	DescriptorHeap* dsvHeap_ = nullptr;

};

