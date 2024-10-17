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

#include "GraphicsPipelineManager.h"
#include "GraphicsCommon.h"

class SwapChain;
class CommandList;
class CommandQueue;
class DepthBuffer;
class BackBuffer;
class DescriptorHeap;

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
	/// ファイナライザ
	/// </summary>
	void Finalize();

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
	/// getter
	/// </summary>
	/// <param name="heapType"></param>
	/// <returns></returns>
	DescriptorHeap* GetDescriptorHeap(HeapType heapType) { return descriptorHeaps_[static_cast<int>(heapType)].get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_->GetCommandList(); }
	UINT GetBackBufferCount() { return backBuffer_->GetBackBufferCount(); }
	

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

	std::unique_ptr<DescriptorHeap> descriptorHeaps_[static_cast<int>(HeapType::kCount)];

	std::chrono::steady_clock::time_point reference_;
};

