#include "T.h"

#include "RootSignature.h"
#include "PipelineState.h"
#include "Shader.h"

void T::Initialize(ID3D12Device* device)
{
	device_ = device;
}

void T::Create()
{
	rootSignature_ = new RootSignature(device_.Get(), 0, 0);
	rootSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	PSO_ = new PipelineState(device_.Get(), rootSignature_->GetSignature());

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	PSO_->SetInputLayout(inputLayoutDesc);

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	PSO_->SetBlendState(blendDesc);

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	PSO_->SetRasterizerState(rasterizerDesc);





}

void T::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHandle)
{


}

void T::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection)
{
}

void T::SetMaterial(const Vector4& color)
{
}

void T::CreateMesh()
{
}

void T::LoadObjFile(const std::string& filename)
{
}

T::MaterialData T::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	return MaterialData();
}

void T::InitializeDirectionalLight()
{
}

void T::InitializeMaterial()
{
}

ID3D12Resource* T::CreateBufferResource(size_t sizeInBytes)
{
	return nullptr;
}
