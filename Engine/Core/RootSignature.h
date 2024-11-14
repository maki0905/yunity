#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <cassert>
#include "RootParameter.h"

class RootParameter;

/*
* @brief ルートシグネチャークラス
*/
class RootSignature
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device"></param>
	/// <param name="numRootParameters"></param>
	/// <param name="numStaticSamplers"></param>
	RootSignature(ID3D12Device* device, UINT numRootParameters = 0, UINT numStaticSamplers = 0);

	/// <summary>
	/// リセット
	/// </summary>
	/// <param name="numRootParameters"></param>
	/// <param name="numStaticSamplers"></param>
	void Reset(UINT numRootParameters, UINT numStaticSamplers = 0);

	/// <summary>
	/// StaticSampler初期化
	/// </summary>
	/// <param name="shaderRegister"></param>
	/// <param name="nonStaticSamplerDesc"></param>
	/// <param name="shaderVisibility"></param>
	void InitializeStaticSampler(UINT shaderRegister, const D3D12_STATIC_SAMPLER_DESC& nonStaticSamplerDesc, D3D12_SHADER_VISIBILITY shaderVisibility);

	/// <summary>
	/// ファイナライザ
	/// </summary>
	/// <param name="flags"></param>
	void Finalize(D3D12_ROOT_SIGNATURE_FLAGS flags);

	/// <summary>
	/// getter
	/// </summary>
	/// <returns></returns>
	ID3D12RootSignature* GetSignature() const { return rootSignature_.Get(); }
	RootParameter& GetParameter(size_t entryIndex) {
		return parameterArray_.get()[entryIndex];
	}
	RootParameter& operator[](size_t entryIndex) {
		assert(entryIndex < numParameters_);
		return parameterArray_.get()[entryIndex];
	}
	const RootParameter& operator[](size_t entryIndex)const {
		assert(entryIndex < numParameters_);
		return parameterArray_.get()[entryIndex];
	}

protected:
	ID3D12Device* device_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	UINT numParameters_;
	UINT numSamplers_;
	UINT numInitializedStaticSamplers_ = 0;
	std::unique_ptr<RootParameter[]> parameterArray_;
	std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]> samplerArray_;

};

