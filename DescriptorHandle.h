//#pragma once
//
//#include <format>
//#include <d3d12.h>
//#include <wrl.h>
//
//#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1)
//
//class DescriptorHandle
//{
//public:
//    DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) 
//        : cpuHandle_(cpuHandle), gpuHandle_(gpuHandle) {}
//
//
//    DescriptorHandle operator+ (INT OffsetScaledByDescriptorSize) const
//    {
//        DescriptorHandle ret = *this;
//        ret += OffsetScaledByDescriptorSize;
//        return ret;
//    }
//
//    void operator += (INT OffsetScaledByDescriptorSize)
//    {
//        if (cpuHandle_.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
//            cpuHandle_.ptr += OffsetScaledByDescriptorSize;
//        if (gpuHandle_.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
//            gpuHandle_.ptr += OffsetScaledByDescriptorSize;
//    }
//
//	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() { return cpuHandle_; }
//	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() { return gpuHandle_; }
//
//	size_t GetCpuPtr() const { return cpuHandle_.ptr; }
//	uint64_t GetGpuPtr() const { return gpuHandle_.ptr; }
//
//
//private:
//	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle_;
//	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_;
//
//};
//
