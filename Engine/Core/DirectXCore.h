#pragma once

#include <d3d12.h>
#include <memory>
#include <vector>
#include <chrono>

#include "Input.h"
#include "Device.h"
#include "ShaderCompiler.h"
#include "CommandList.h"
#include "ImGuiManager.h"
#include "BackBuffer.h"
#include "RenderTexture.h"
#include "SwapChain.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "DepthBuffer.h"
#include "BackBuffer.h"
#include "DescriptorHeap.h"

#include "GraphicsPipelineManager.h"
#include "GraphicsCommon.h"

namespace yunity {
	/*
	* @brief DirectXCoreクラス
	*/
	class DirectXCore
	{
	public:
		enum class HeapType {
			kRTV,
			kSRV,
			kDSV,
			kCount
		};

	public:
		/// <summary>
		/// インスタンス取得
		/// </summary>
		/// <returns></returns>
		static DirectXCore* GetInstance();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// レンダーテクスチャ事前処理
		/// </summary>
		void PreDrawRenderTexture();

		/// <summary>
		/// レンダーテクスチャ事後処理
		/// </summary>
		void PostDrawRenderTexture();

		/// <summary>
		/// スワップチェーン事前処理
		/// </summary>
		void PreDrawSwapchain();

		/// <summary>
		/// スワップチェーン事後処理
		/// </summary>
		void PostDrawSwapchain();

		/// <summary>
		/// シャドウ描画前処理
		/// </summary>
		void PreDrawShadow();

		/// <summary>
		/// シャドウ描画後処理
		/// </summary>
		void PostDrawShadow();

		/// <summary>
		/// 各ヒープ生成
		/// </summary>
		void CreateDescriptorHeaps();


		/// <summary>
		/// getter
		/// </summary>
		/// <param name="heapType"></param>
		/// <returns></returns>
		yunity::DescriptorHeap* GetDescriptorHeap(HeapType heapType) { return descriptorHeaps_[static_cast<int>(heapType)].get(); }
		ID3D12GraphicsCommandList* GetCommandList() { return commandList_->GetCommandList(); }
		UINT GetBackBufferCount() { return backBuffer_->GetBackBufferCount(); }
		D3D12_GPU_DESCRIPTOR_HANDLE GetShdowHandle() { return shadowGpuDescHandleSRV_; }


	private:
		/// <summary>
		/// 固定FPS初期化
		/// </summary>
		void InitializeFixFPS();

		/// <summary>
		/// 固定FPS更新
		/// </summary>
		void UpdateFixFPS();

	private:
		DirectXCore() = default;
		~DirectXCore() = default;
		DirectXCore(const DirectXCore&) = delete;
		const DirectXCore& operator=(const DirectXCore&) = delete;
	private:
		std::unique_ptr<CommandList> commandList_;
		std::unique_ptr<CommandQueue> commandQueue_;
		std::unique_ptr<SwapChain> swapChain_;
		std::unique_ptr<BackBuffer> backBuffer_;
		std::unique_ptr<DepthBuffer> depthBuffer_;

		UINT windowWidth_;
		UINT windowHeight_;

		std::unique_ptr<yunity::DescriptorHeap> descriptorHeaps_[static_cast<int>(HeapType::kCount)];

		std::chrono::steady_clock::time_point reference_;

		// シャドウバッファ
		std::unique_ptr<DepthBuffer> shadowBuffer_;
		// シェーダリソースビューのハンドル(CPU)
		D3D12_CPU_DESCRIPTOR_HANDLE shadowCpuDescHandleSRV_;
		// シェーダリソースビューのハンドル(GPU)
		D3D12_GPU_DESCRIPTOR_HANDLE shadowGpuDescHandleSRV_;
	};

}

