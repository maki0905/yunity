#include "RootSignature.h"

#include "RootParameter.h"

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
		numParameters_ = numRootParameters;
	}

	if (numStaticSamplers > 0) {
		samplerArray_.reset(new D3D12_STATIC_SAMPLER_DESC[numStaticSamplers]);
	}
	else {
		samplerArray_ = nullptr;
		numSamplers_ = numStaticSamplers;
		numInitializedStaticSamplers = 0;

	}

}

void RootSignature::Finalize(D3D12_ROOT_SIGNATURE_FLAGS flags)
{
	HRESULT result_ = S_FALSE;

	assert(numInitializedStaticSamplers == 0);

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
		assert(false);
	}

	rootSignature_ = nullptr;
	result_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
}