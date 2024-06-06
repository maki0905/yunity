#include "GraphicsPipelineManager.h"

#include "Device.h"
#include "ShaderCompiler.h"

void GraphicsPipelineManager::Initialize()
{
	device_ = Device::GetInstance()->GetDevice();
	graphicsCommon_ = std::make_unique<GraphicsCommon>();
	graphicsCommon_->Initialize();

	for (uint32_t pipelineType = 0; pipelineType < PipelineType::kCount; pipelineType++) {
		graphicsPipelines_[pipelineType] = new GraphicsPipeline();
	}
	D3D12_BLEND_DESC blendDesc[BlendModeType::kBlendCount];
	blendDesc_[0] = graphicsCommon_->NoneBlend;
	blendDesc_[1] = graphicsCommon_->NormalBlend;
	blendDesc_[2] = graphicsCommon_->AddBlend;
	blendDesc_[3] = graphicsCommon_->SubtractBlend;
	blendDesc_[4] = graphicsCommon_->MultilyBlend;
	blendDesc_[5] = graphicsCommon_->ScreenBlend;

	CreateObject3d();
	CreateTexture();
	CreateParticle();
	CreatePrimitive();
	CreateSkinning();

}

void GraphicsPipelineManager::SetCommandList(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendModeType blendModeType)
{
	commandList->SetGraphicsRootSignature(graphicsPipelines_[pipelineType]->rooSignature_->GetSignature());
	commandList->SetPipelineState(graphicsPipelines_[pipelineType]->pso_[blendModeType]->GetPipelineStateObject());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GraphicsPipelineManager::CreateObject3d()
{
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_ = new RootSignature(device_, 8, 1);
	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(0).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(1).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(2).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(3).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(4).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(5).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(6).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(7).InitializeAsConstantBuffer(3, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC object3DInputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "TEXCOORD", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "NORMAL", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	inputLayoutDesc.pInputElementDescs = object3DInputElementDescs;
	inputLayoutDesc.NumElements = _countof(object3DInputElementDescs);

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc = GraphicsPipelineManager::graphicsCommon_->RasterizerDefault;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = GraphicsPipelineManager::graphicsCommon_->DepthStateReadWrite;

	for (uint32_t blendModeType = 0; blendModeType < BlendModeType::kBlendCount; blendModeType++) {
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType] = new PipelineState(device_, graphicsPipelines_[PipelineType::kObject3d]->rooSignature_);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetInputLayout(inputLayoutDesc);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kBasic, ShaderCompiler::ShaderType::kVS));
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kBasic, ShaderCompiler::ShaderType::kPS));
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetBlendState(blendDesc_[blendModeType]);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetRasterizerState(rasterizerDesc);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetDepthStencilState(depthStencilDesc);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->Finalize();
	}
}

void GraphicsPipelineManager::CreateTexture()
{
}

void GraphicsPipelineManager::CreateParticle()
{
}

void GraphicsPipelineManager::CreatePrimitive()
{
}

void GraphicsPipelineManager::CreateSkinning()
{
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_ = new RootSignature(device_, 9, 1);
	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(0).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(1).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(2).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(3).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(4).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(5).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(6).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(7).InitializeAsConstantBuffer(3, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(8).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC  SkinningInputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "TEXCOORD", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "NORMAL", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "WEIGHT", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .InputSlot = 1, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "INDEX", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_SINT, .InputSlot = 1, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT },
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	inputLayoutDesc.pInputElementDescs = SkinningInputElementDescs;
	inputLayoutDesc.NumElements = _countof(SkinningInputElementDescs);

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc = GraphicsPipelineManager::graphicsCommon_->RasterizerDefault;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = GraphicsPipelineManager::graphicsCommon_->DepthStateReadWrite;

	for (uint32_t blendModeType = 0; blendModeType < BlendModeType::kBlendCount; blendModeType++) {
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType] = new PipelineState(device_, graphicsPipelines_[PipelineType::kSkinning]->rooSignature_);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetInputLayout(inputLayoutDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kSkinning, ShaderCompiler::ShaderType::kVS));
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kBasic, ShaderCompiler::ShaderType::kPS));
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetBlendState(blendDesc_[blendModeType]);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetRasterizerState(rasterizerDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetDepthStencilState(depthStencilDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->Finalize();
	}
}

GraphicsPipelineManager* GraphicsPipelineManager::GetInstance()
{
	static GraphicsPipelineManager instance;
	return &instance;
}
