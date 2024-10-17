#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <cmath>

#include "MathFunction.h"
#include "DepthBuffer.h"
#include "PostEffect.h"

class PipelineState;
class RootSignature;

/*
* @brief レンダーテクスチャクラス
*/
class RenderTexture {
	/// <summary>
	/// ルートパラメータ番号
	/// </summary>
	enum class RootBindings {
		kTexture,        // テクスチャ
		kDepthTexture,   // depth
		kMaterial,
		kCount,          // 最大数
	};

public:
	// 
	struct Material {
		Matrix4x4 projectionInverse;
	};
public:
	/// <summary>
	/// グラフィックパイプライン初期化
	/// </summary>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static RenderTexture* GetInstance();
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
	/// 生成
	/// </summary>
	void Create();

	/// <summary>
	/// 画面加工
	/// </summary>
	void Machining();

	/// <summary>
	/// コピー
	/// </summary>
	void Copy();

	/// <summary>
	/// ポストエフェクトチェック
	/// </summary>
	/// <returns></returns>
	bool CheckPostEffect();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	//void OMSetREnderTargets(ID3D12DescriptorHeap* dsvHeap_);

	/// <summary>
	/// ClearRenderTargetView設定
	/// </summary>
	void ClearRenderTargetView();

	/// <summary>
	/// OMSetRenderTargets設定
	/// </summary>
	void OMSetRenderTargets();

	/// <summary>
	/// ClearDepthStencilView設定
	/// </summary>
	void ClearDepthStencilView();

	/// <summary>
	/// ポストエフェクトクリア
	/// </summary>
	void ClearPostEffect();

	/// <summary>
	/// ポストエフェクト選択
	/// </summary>
	/// <param name="postEffect"></param>
	/// <param name="flag"></param>
	void SelectPostEffect(PostEffects postEffect, bool flag);


	/// <summary>
	/// getter
	/// </summary>
	/// <returns></returns>
	Vector4 GetRenderTargetClearValue() { return kRenderTargetClearValue; }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetCpuDescHandleRTV() { return &cpuDescHandleRTV_; }
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDepthCpuDescHandleRTV() { return &depthTextureCpuDescHandleSRV_; }
	ID3D12Resource* GetResource() { return renderTextureResource_.Get(); }
	DepthBuffer* GetDepthBuffer() { return depthBuffe_.get(); }
	PostEffect* GetPostEffect() { return postEffect_.get(); }
	
	
	bool postEffectFlag_[static_cast<uint32_t>(PostEffects::kCount)];

private:
	/// <summary>
	/// リソース生成
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

	/// <summary>
	/// RenderTextureResource生成
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
	RenderTexture() = default;
	~RenderTexture() = default;
	RenderTexture(const RenderTexture&) = delete;
	RenderTexture& operator=(const RenderTexture&) = delete;
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
	std::unique_ptr<PostEffect> postEffect_;

	bool selectedFlag_;
};
