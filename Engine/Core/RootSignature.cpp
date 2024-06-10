#include "RootSignature.h"

#include "RootParameter.h"
#include "Logger.h"

#include <cassert>

#pragma comment(lib, "d3d12.lib")


RootSignature::RootSignature(ID3D12Device* device, UINT numRootParameters, UINT numStaticSamplers)
{
	device_ = device;
	Reset(numRootParameters, numStaticSamplers);
}

void RootSignature::Reset(UINT numRootParameters, UINT numStaticSamplers)
{
	if (numRootParameters > 0) {
		parameterArray_.reset(new RootParameter[numRootParameters]);
	}
	else {
		parameterArray_ = nullptr;
	}
	numParameters_ = numRootParameters;

	if (numStaticSamplers > 0) {
		samplerArray_.reset(new D3D12_STATIC_SAMPLER_DESC[numStaticSamplers]);
	}
	else {
		samplerArray_ = nullptr;
	}
	numSamplers_ = numStaticSamplers;
	numInitializedStaticSamplers_ = 0;

}

void RootSignature::InitializeStaticSampler(UINT shaderRegister, const D3D12_STATIC_SAMPLER_DESC& nonStaticSamplerDesc, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	assert(numInitializedStaticSamplers_ < numSamplers_);
	D3D12_STATIC_SAMPLER_DESC& staticSamplerDesc = samplerArray_[numInitializedStaticSamplers_++];

	staticSamplerDesc.Filter = nonStaticSamplerDesc.Filter;
	staticSamplerDesc.AddressU = nonStaticSamplerDesc.AddressU;
	staticSamplerDesc.AddressV = nonStaticSamplerDesc.AddressV;
	staticSamplerDesc.AddressW = nonStaticSamplerDesc.AddressW;
	staticSamplerDesc.MipLODBias = nonStaticSamplerDesc.MipLODBias;
	staticSamplerDesc.MaxAnisotropy = nonStaticSamplerDesc.MaxAnisotropy;
	staticSamplerDesc.ComparisonFunc = nonStaticSamplerDesc.ComparisonFunc;
	staticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	staticSamplerDesc.MinLOD = nonStaticSamplerDesc.MinLOD;
	staticSamplerDesc.MaxLOD = nonStaticSamplerDesc.MaxLOD;
	staticSamplerDesc.ShaderRegister = shaderRegister;
	staticSamplerDesc.RegisterSpace = 0;
	staticSamplerDesc.ShaderVisibility = shaderVisibility;


}

void RootSignature::Finalize(D3D12_ROOT_SIGNATURE_FLAGS flags)
{
	HRESULT result_ = S_FALSE;

	assert(numInitializedStaticSamplers_ == numSamplers_);

	D3D12_ROOT_SIGNATURE_DESC rootDesc;
	rootDesc.NumParameters = numParameters_;
	rootDesc.pParameters = (const D3D12_ROOT_PARAMETER*)parameterArray_.get();
	rootDesc.NumStaticSamplers = numSamplers_;
	rootDesc.pStaticSamplers = (const D3D12_STATIC_SAMPLER_DESC*)samplerArray_.get();
	rootDesc.Flags = flags;

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	result_ = D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(result_)) {
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());

		assert(false);
	}

	rootSignature_ = nullptr;
	result_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
}