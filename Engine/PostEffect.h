#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <cmath>

#include "MathFunction.h"
#include "DepthBuffer.h"

class PipelineState;
class RootSignature;

class PostEffect {
	/// <summary>
	/// ルートパラメータ番号
	/// </summary>
	enum class RootBindings {
		kTexture,        // テクスチャ
		kDepthTexture,   // depth
		kMaterial,
		kCount,          // 最大数
	};

	enum class PostEffects {
		kGrayscale,
		kVignetting,
		kSmoothing,
		kGaussianFilter,
		kOutline,
		kRadialBlur,
		kDissolve,
		kRandom,
	};

public:
	struct Material {
		Matrix4x4 projectionInverse;
	};
public:
	static void InitializeGraphicsPipeline();
public:
	void Initalize();

private:

	void CreateResorce();
	void CreateRTV();
	void CreateSRV();

	void InitializeMaterial();

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t hight, DXGI_FORMAT format, const Vector4& clearColor);

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns>サイズ</returns>
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);
private:
	static RootSignature* rootSignature_;
	static PipelineState* pipelineState_;

private:
	const Vector4 kRenderTargetClearValue{ 1.0f, 0.0f, 0.0f, 1.0f };
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleRTV_;
	// シェーダリソースビューのハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
	// DepthTexture用のシェーダリソースビューのハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE depthTextureCpuDescHandleSRV_;
	// DepthTexture用のシェーダリソースビューのハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE depthTextureGpuDescHandleSRV_;

	// マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	// 深度バッファクラス
	std::unique_ptr<DepthBuffer> depthBuffe_;
};