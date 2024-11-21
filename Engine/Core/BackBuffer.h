#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <dxgi1_6.h>
#include <memory>

#include "DescriptorHeap.h"

//class yunity::DescriptorHeap;

namespace yunity {
	/*
	* @brief バックバッファクラス
	*/
	class BackBuffer
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="device"></param>
		/// <param name="commandList"></param>
		/// <param name="swapChain"></param>

		void Finalize();
		/// <summary>
		/// 生成
		/// </summary>
		void Create(IDXGISwapChain4* swapChain);

		/// <summary>
		/// レンダーターゲット設定
		/// </summary>
		/// <param name="swapChain"></param>
		/// <param name="dsvHeap_"></param>
		void SetRenderTarget(ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain, ID3D12DescriptorHeap* dsvHeap_);

		/// <summary>
		/// レンダーターゲットクリア
		/// </summary>
		/// <param name="swapChain"></param>
		void ClearRenderTarget(ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain);

		/// <summary>
		/// getter
		/// </summary>
		/// <returns></returns>

		UINT GetBackBufferCount() { return backBufferCount_; }
		D3D12_CPU_DESCRIPTOR_HANDLE* GetCpuDescHandleRTV() { return rtvHandles_; }

	private:
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;
		DescriptorHeap* rtvDescriptorHeap_ = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
		UINT backBufferCount_ = 0;

	};
}


