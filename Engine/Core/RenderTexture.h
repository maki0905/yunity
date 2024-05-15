#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <cmath>

#include "Vector4.h"

class PipelineState;
class RootSignature;

class RenderTexture {
	/// <summary>
	/// ルートパラメータ番号
	/// </summary>
	enum class RootBindings {
		kTexture,        // テクスチャ
		kCount,          // 最大数
	};
public:
	static void InitializeGraphicsPipeline();
public:
	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	void Copy();

	void OMSetREnderTargets(ID3D12DescriptorHeap* dsvHeap_);
	void ClearRenderTargetView();

	Vector4 GetRenderTargetClearValue() { return kRenderTargetClearValue; }

	D3D12_CPU_DESCRIPTOR_HANDLE* GetCpuDescHandleRTV() { return &cpuDescHandleRTV_; }

	ID3D12Resource* GetResource() { return renderTextureResource.Get(); }

private:

	void CreateResorce();
	void CreateRTV();
	void CreateSRV();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t hight, DXGI_FORMAT format, const Vector4& clearColor);
private:
	static RootSignature* rootSignature_;
	static PipelineState* pipelineState_;

private:
	const Vector4 kRenderTargetClearValue{ 1.0f, 0.0f, 0.0f, 1.0f };
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleRTV_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	// シェーダリソースビューのハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
};
