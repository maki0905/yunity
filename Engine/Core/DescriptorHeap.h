#pragma once

#include <format>
#include <d3d12.h>
#include <wrl.h>

#include "DescriptorHandle.h"

namespace yunity {
	/*
	* @brief ディスクリプタヒープクラス
	*/
	class DescriptorHeap
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		DescriptorHeap() : descriptorSize_(0), freeDescriptors_(0) {};

		/// <summary>
		/// デストラクタ
		/// </summary>
		~DescriptorHeap() { Destroy(); }

		/// <summary>
		/// 生成
		/// </summary>
		/// <param name="heapType"></param>
		/// <param name="numDescriptors"></param>
		/// <param name="shaderVisible"></param>
		void Create(D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t numDescriptors, bool shaderVisible);

		/// <summary>
		/// 破棄
		/// </summary>
		void Destroy() { heap_ = nullptr; }

		/// <summary>
		/// 空きスペース取得
		/// </summary>
		/// <param name="count"></param>
		/// <returns></returns>
		bool HasAvailableSpace(uint32_t count) const { return count <= freeDescriptors_; }

		/// <summary>
		/// アロケータ
		/// </summary>
		/// <param name="count"></param>
		/// <returns></returns>
		DescriptorHandle Alloc(uint32_t count = 1);

		/// <summary>
		/// ハンドル設定
		/// </summary>
		/// <param name="DHandle"></param>
		/// <returns></returns>
		bool ValidateHandle(const DescriptorHandle& DHandle) const;

		/// <summary>
		/// getter
		/// </summary>
		/// <returns></returns>

		uint32_t GetOffsetOfHandle(const DescriptorHandle& DHandle) {
			return (uint32_t)(DHandle.GetCpuPtr() - firstHandle_->GetCpuPtr()) / descriptorSize_;
		}
		ID3D12DescriptorHeap* GetHeapPointer() const { return heap_.Get(); }
		uint32_t GetDescriptorSize(void) const { return descriptorSize_; }




	private:

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc_;
		uint32_t descriptorSize_;
		uint32_t freeDescriptors_;
		DescriptorHandle* firstHandle_;
		DescriptorHandle* nextFreeHandle_;

	};
}



