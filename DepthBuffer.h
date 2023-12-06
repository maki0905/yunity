#pragma once

#include <d3d12.h>
#include <wrl.h>

class DepthBuffer
{
public:

	DepthBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	void Create();

	void ClearDepthView();

	ID3D12DescriptorHeap* GetDescriptorHeap() { return dsvHeap_.Get(); }

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

private:

	Microsoft::WRL::ComPtr<ID3D12Device> device_;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;

};

