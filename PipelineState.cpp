#include "PipelineState.h"

#include <cassert>

#include "RootSignature.h"

PipelineState::PipelineState(ID3D12Device* device, RootSignature* rootSignature)
{
	device_ = device;
	rootSignature_ = rootSignature;
}

void PipelineState::SetBlendState(const D3D12_BLEND_DESC& blendDesc)
{
	psoDesc_.BlendState = blendDesc;
}

void PipelineState::SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc)
{
	psoDesc_.RasterizerState = rasterizerDesc;
}

void PipelineState::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	psoDesc_.DepthStencilState = depthStencilDesc;
}

void PipelineState::SetSampleMask(UINT sampleMask)
{
	psoDesc_.SampleMask = sampleMask;
}

void PipelineState::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType)
{
	assert(topologyType != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED);
	psoDesc_.PrimitiveTopologyType = topologyType;
}

void PipelineState::SetDepthTargetFormat(DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
{
	SetRenderTargetFormats(0, nullptr, dsvFormat, msaaCount, msaaQuality);
}

void PipelineState::SetRenderTargetFormat(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
{
	SetRenderTargetFormats(1, &rtvFormat, dsvFormat, msaaCount, msaaQuality);
}

void PipelineState::SetRenderTargetFormats(UINT numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
{
	assert(numRTVs == 0 || rtvFormats != nullptr);
	for (UINT i = 0; i < numRTVs; ++i) {
		assert(rtvFormats[i] != DXGI_FORMAT_UNKNOWN);
		psoDesc_.RTVFormats[i] = rtvFormats[i];
	}

	for (UINT i = numRTVs; i < psoDesc_.NumRenderTargets; ++i) {
		psoDesc_.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
	}

	psoDesc_.NumRenderTargets = numRTVs;
	psoDesc_.DSVFormat = dsvFormat;
	psoDesc_.SampleDesc.Count = msaaCount;
	psoDesc_.SampleDesc.Quality = msaaQuality;
}

void PipelineState::SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc)
{
	psoDesc_.InputLayout = inputLayoutDesc;
}

void PipelineState::SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps)
{
	psoDesc_.IBStripCutValue = IBProps;
}

void PipelineState::SetShader(ShaderType type, Microsoft::WRL::ComPtr<ID3DBlob> blob)
{
	switch (type)
	{
	case PipelineState::ShaderType::kVS:
		psoDesc_.VS = { blob->GetBufferPointer(), blob->GetBufferSize() };
		break;
	case PipelineState::ShaderType::kPS:
		psoDesc_.PS = { blob->GetBufferPointer(), blob->GetBufferSize() };
		break;
	}
}

void PipelineState::Finalize()
{
	HRESULT result = S_FALSE;

	psoDesc_.pRootSignature = rootSignature_->GetSignature();

	graphicsPipelineState_ = nullptr;

	result = device_->CreateGraphicsPipelineState(&psoDesc_, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(result));
}