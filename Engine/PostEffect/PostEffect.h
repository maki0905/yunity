#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <cmath>

#include "MathFunction.h"
#include "DepthBuffer.h"

class PipelineState;
class RootSignature;

enum class PostEffects {
	kOutline,
	kGrayscale,
	kSmoothing,
	kGaussianFilter,
	kRadialBlur,
	kVignetting,
	kDissolve,
	kRandom,
	kHSVFilter,
	kCount,
};

/*
* @brief ポストエフェクトクラス
*/
class PostEffect {
public:
	/// <summary>
	/// ルートパラメータ番号
	/// </summary>
	enum class RootBindings {
		kTexture,        // テクスチャ
		kDepthTexture,   // depth
		kMaterial,
		kCount,          // 最大数
	};
	struct Material {
		Matrix4x4 projectionInverse;
		float hue;
		float saturation;
		float value;
	};
public:

	/// <summary>
	/// パイプライン初期化
	/// </summary>
	static void InitializeGraphicsPipeline();
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();
	
	/// <summary>
	/// ファイナライザ
	/// </summary>
	void Finalize();
	
	/// <summary>
	/// RootSignature設定
	/// </summary>
	/// <param name="index"></param>
	void SetGraphicsRootSignature(uint32_t index);
	
	/// <summary>
	/// PipelineState設定
	/// </summary>
	/// <param name="index"></param>
	void SetPipelineState(uint32_t index);
	
	/// <summary>
	/// クリアカラー設定
	/// </summary>
	/// <param name="index"></param>
	void CRTV(uint32_t index);
	
	/// <summary>
	/// RenderTargetViewクリア
	/// </summary>
	/// <param name="index"></param>
	void ClearRenderTargetView(uint32_t index);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="index"></param>
	void OMSetRenderTargets(uint32_t index);
	
	/// <summary>
	/// DepthStencilView生成
	/// </summary>
	void ClearDepthStencilView();
	
	/// <summary>
	/// DescriptorTable設定
	/// </summary>
	/// <param name="binding"></param>
	/// <param name="index"></param>
	void SetGraphicsRootDescriptorTable(RootBindings binding, uint32_t index);
	
	/// <summary>
	/// マテリアル設定
	/// </summary>
	/// <param name="index"></param>
	void SetMaterial(uint32_t index);
	
	/// <summary>
	/// マテリアルデータ設定
	/// </summary>
	/// <param name="index"></param>
	void SetMaterialData(uint32_t index);
	
	/// <summary>
	/// ImGui
	/// </summary>
	void Adjustment();

	//private:
	//	PostEffect() = default;
	//	~PostEffect() = default;
	//	PostEffect(const PostEffect&) = delete;
	//	PostEffect& operator=(const PostEffect&) = delete;
private:

	/// <summary>
	/// Resorce生成
	/// </summary>
	void CreateResorce();
	
	/// <summary>
	/// RTV生成
	/// </summary>
	void CreateRTV();
	
	/// <summary>
	/// SRV生成
	/// </summary>
	void CreateSRV();

	/// <summary>
	/// マテリアル初期化
	/// </summary>
	void InitializeMaterial();

	//void ClearRenderTargetView(uint32_t flag);

	/// <summary>
	/// レンダーテクスチャ生成
	/// </summary>
	/// <param name="width"></param>
	/// <param name="hight"></param>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t hight, DXGI_FORMAT format, const Vector4& clearColor);

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns>サイズ</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
private:
	static RootSignature* rootSignature_[static_cast<uint32_t>(PostEffects::kCount)];
	static PipelineState* pipelineState_[static_cast<uint32_t>(PostEffects::kCount)];

private:
	const Vector4 kRenderTargetClearValue{ 1.0f, 0.0f, 0.0f, 1.0f };
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_[static_cast<uint32_t>(PostEffects::kCount)];
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleRTV_[static_cast<uint32_t>(PostEffects::kCount)];
	// シェーダリソースビューのハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_[static_cast<uint32_t>(PostEffects::kCount)];
	// シェーダリソースビューのハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_[static_cast<uint32_t>(PostEffects::kCount)];
	// DepthTexture用のシェーダリソースビューのハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE depthTextureCpuDescHandleSRV_;
	// DepthTexture用のシェーダリソースビューのハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE depthTextureGpuDescHandleSRV_;

	// マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_[static_cast<uint32_t>(PostEffects::kCount)];
	Material* materialData_[static_cast<uint32_t>(PostEffects::kCount)];
	//Material* materialData_;
	// 深度バッファクラス
	std::unique_ptr<DepthBuffer> depthBuffe_;
};