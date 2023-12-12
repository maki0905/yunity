#pragma once

#include <format>
#include <d3d12.h>
#include <wrl.h>

#include "DescriptorHandle.h"

class DescriptorHeap
{
public:
	
	~DescriptorHeap() { Destroy(); }

	void Create(D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t numDescriptors, bool shaderVisible);
	void Destroy() { heap_ = nullptr; }

	bool HasAvailableSpace(uint32_t count) const { return count <= freeDescriptors_; }
	DescriptorHandle Alloc(uint32_t count = 1);

	uint32_t GetOffsetOfHandle(const DescriptorHandle& DHandle) {
		return (uint32_t)(DHandle.GetCpuPtr() - firstHandle_.GetCpuPtr()) / descriptorSize_;
	}

	bool ValidateHandle(const DescriptorHandle& DHandle) const;

	ID3D12DescriptorHeap* GetHeapPointer() const { return heap_.Get(); }


	uint32_t GetDescriptorSize(void) const { return descriptorSize_; }


private:

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc_;
	uint32_t descriptorSize_;
	uint32_t freeDescriptors_;
	DescriptorHandle firstHandle_;
	DescriptorHandle nextFreeHandle_;

};

