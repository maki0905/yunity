#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "DescriptorHeap.h"

class DescriptorHeap;

namespace yunity {
	/*
	* @brief 深度バッファ
	*/
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
		/// 初期化
		/// </summary>
		void Initialize(DXGI_FORMAT format, D3D12_RESOURCE_STATES state);

		/// <summary>
		/// デプスビュークリア
		/// </summary>
		void ClearDepthView();


		/// <summary>
		/// getter
		/// </summary>
		/// <returns></returns>

		ID3D12DescriptorHeap* GetDescriptorHeap() { return dsvHeap_->GetHeapPointer(); }
		ID3D12Resource* GetDepthStencil() { return depthStencilResource_.Get(); }

	private:

		/// <summary>
		/// デプスステンシル生成
		/// </summary>
		/// <returns></returns>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(DXGI_FORMAT format, D3D12_RESOURCE_STATES state);

	private:

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;

		DescriptorHeap* dsvHeap_ = nullptr;

	};

}
