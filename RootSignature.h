#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <memory>

class RootParameter;

class RootSignature
{
public:
	RootSignature(ID3D12Device* device, UINT numRootParameters = 0, UINT numStaticSamplers = 0);

	void Reset(UINT numRootParameters, UINT numStaticSamplers = 0);

	void Finalize(D3D12_ROOT_SIGNATURE_FLAGS flags);

	ID3D12RootSignature* GetSignature() const { return rootSignature_.Get(); }
protected:
	ID3D12Device* device_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	UINT numParameters_;
	UINT numSamplers_;
	UINT numInitializedStaticSamplers = 0;
	std::unique_ptr<RootParameter[]> parameterArray_;
	std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]> samplerArray_;

};

