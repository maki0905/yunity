#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <cmath>

#include "Vector4.h"

class RenderTexture {
public:
	static void StaticInitialize();

public:
	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	void OMSetREnderTargets(ID3D12DescriptorHeap* dsvHeap_);
	void ClearRenderTargetView();
private:
	
	void CreateResorce();
	void CreateRTV();
	void CreateSRV();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t hight, DXGI_FORMAT format, const Vector4& clearColor);
	

private:
	static ID3D12Device* device_;
	const Vector4 kRenderTargetClearValue{ 1.0f, 0.0f, 0.0f, 1.0f };
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleRTV_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;


};
