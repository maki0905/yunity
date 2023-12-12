#include "DescriptorHeap.h"

#include <cassert>

#include "Device.h"

#pragma comment(lib, "d3d12.lib")







void DescriptorHeap::Create(D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t numDescriptors, bool shaderVisible)
{
	Device* device = Device::GetInstance();

	heapDesc_.Type = heapType;
	heapDesc_.NumDescriptors = numDescriptors;
	heapDesc_.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc_.NodeMask = 1;

	HRESULT result = device->GetDevice()->CreateDescriptorHeap(&heapDesc_, IID_PPV_ARGS(&heap_));
	assert(SUCCEEDED(result));

	descriptorSize_ = device->GetDevice()->GetDescriptorHandleIncrementSize(heapDesc_.Type);
	freeDescriptors_ = heapDesc_.NumDescriptors;

	if (shaderVisible) {
		firstHandle_ = new DescriptorHandle(heap_->GetCPUDescriptorHandleForHeapStart(), heap_->GetGPUDescriptorHandleForHeapStart());
	}
	else {
		firstHandle_ = new DescriptorHandle(heap_->GetCPUDescriptorHandleForHeapStart());
	}

	nextFreeHandle_ = firstHandle_;
}

DescriptorHandle DescriptorHeap::Alloc(uint32_t count)
{
	assert(HasAvailableSpace(count));
	DescriptorHandle ret = *nextFreeHandle_;
	nextFreeHandle_ += count * descriptorSize_;
	freeDescriptors_ -= count;
	return ret;
}

bool DescriptorHeap::ValidateHandle(const DescriptorHandle& DHandle) const
{
	if (DHandle.GetCpuPtr() < firstHandle_->GetCpuPtr() ||
		DHandle.GetCpuPtr() >= firstHandle_->GetCpuPtr() + heapDesc_.NumDescriptors * descriptorSize_)
		return false;

	if (DHandle.GetGpuPtr() - firstHandle_->GetGpuPtr() !=
		DHandle.GetCpuPtr() - firstHandle_->GetCpuPtr())
		return false;

	return true;


}
