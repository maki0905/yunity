#include "RootParameter.h"
#include "RootSignature.h"

#include <cassert>

#pragma comment(lib, "d3d12.lib")



yunity::RootParameter::RootParameter()
{
	rootParameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
}

yunity::RootParameter::~RootParameter()
{
	Clear();
}

void yunity::RootParameter::Clear()
{
	if (rootParameter_.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE) {
		delete[] rootParameter_.DescriptorTable.pDescriptorRanges;
	}
	rootParameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
}

void yunity::RootParameter::InitializeAsConstants(UINT shaderRegister, UINT numDwords, D3D12_SHADER_VISIBILITY shaderVisibility, UINT registerSpace)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParameter_.ShaderVisibility = shaderVisibility;
	rootParameter_.Constants.Num32BitValues = numDwords;
	rootParameter_.Constants.ShaderRegister = shaderRegister;
	rootParameter_.Constants.RegisterSpace = registerSpace;

}

void yunity::RootParameter::InitializeAsConstantBuffer(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility, UINT registerSpace)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter_.ShaderVisibility = shaderVisibility;
	rootParameter_.Descriptor.ShaderRegister = shaderRegister;
	rootParameter_.Descriptor.RegisterSpace = registerSpace;

}

void yunity::RootParameter::InitializeAsBufferSRV(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility, UINT registerSpace)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParameter_.ShaderVisibility = shaderVisibility;
	rootParameter_.Descriptor.ShaderRegister = shaderRegister;
	rootParameter_.Descriptor.RegisterSpace = registerSpace;

}

void yunity::RootParameter::InitializeAsBufferUAV(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility, UINT registerSpace)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameter_.ShaderVisibility = shaderVisibility;
	rootParameter_.Descriptor.ShaderRegister = shaderRegister;
	rootParameter_.Descriptor.RegisterSpace = registerSpace;

}

void yunity::RootParameter::InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT shaderRegister, UINT numDescriptors, D3D12_SHADER_VISIBILITY shaderVisibility, UINT registerSpace)
{
	InitializeAsDescriptorTable(1, shaderVisibility);
	SetTableRange(0, rangeType, shaderRegister, numDescriptors, registerSpace);
}

void yunity::RootParameter::InitializeAsDescriptorTable(UINT rangeCount, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter_.ShaderVisibility = shaderVisibility;
	rootParameter_.DescriptorTable.NumDescriptorRanges = rangeCount;
	rootParameter_.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[rangeCount];

}

void yunity::RootParameter::SetTableRange(UINT rangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT shaderRegister, UINT numDescriptors, UINT registerSpace)
{
	D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(rootParameter_.DescriptorTable.pDescriptorRanges + rangeIndex);
	range->RangeType = rangeType;
	range->NumDescriptors = numDescriptors;
	range->BaseShaderRegister = shaderRegister;
	range->RegisterSpace = registerSpace;
	range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

}