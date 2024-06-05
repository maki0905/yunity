#include "GraphicsPipelineManager.h"

#include "Device.h"
#include "ShaderCompiler.h"

void GraphicsPipelineManager::Initialize()
{
	ID3D12Device* device = Device::GetInstance()->GetDevice();

	graphicsCommon_ = std::make_unique<GraphicsCommon>();
	graphicsCommon_->Initialize();

	for (uint32_t pipelineType = 0; pipelineType < PipelineType::kCount; pipelineType++) {
		graphicsPipelines_[pipelineType] = new GraphicsPipeline();
		/*for (uint32_t blendModeType = 0; blendModeType < BlendModeType::kBlendCount; blendModeType++) {

		}*/

	}

	D3D12_BLEND_DESC blendDesc[BlendModeType::kBlendCount];
	blendDesc[0] = graphicsCommon_->NoneBlend;
	blendDesc[1] = graphicsCommon_->NormalBlend;
	blendDesc[2] = graphicsCommon_->AddBlend;
	blendDesc[3] = graphicsCommon_->SubtractBlend;
	blendDesc[4] = graphicsCommon_->MultilyBlend;
	blendDesc[5] = graphicsCommon_->ScreenBlend;

	// 3d
	/*RootSignature* rootSignature = new RootSignature(device, 9, 1);
	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootSignature->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	rootSignature->GetParameter(0).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->GetParameter(1).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->GetParameter(2).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->GetParameter(3).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature->GetParameter(4).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature->GetParameter(5).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature->GetParameter(6).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature->GetParameter(7).InitializeAsConstantBuffer(3, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature->GetParameter(8).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);*/
	//rootSignatures_[PipelineType::kSkinning] = rootSignature;

	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_ = new RootSignature(device, 9, 1);
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
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "TEXCOORD", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "NORMAL", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "WEIGHT", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .InputSlot = 1, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "INDEX", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_SINT, .InputSlot = 1, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc = GraphicsPipelineManager::graphicsCommon_->RasterizerDefault;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = GraphicsPipelineManager::graphicsCommon_->DepthStateReadWrite;

	for (uint32_t blendModeType = 0; blendModeType < BlendModeType::kBlendCount; blendModeType++) {
		/*PipelineState* pipelineState = new PipelineState(device, rootSignature);
		pipelineState->SetInputLayout(inputLayoutDesc);
		pipelineState->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kSkinning, ShaderCompiler::ShaderType::kVS));
		pipelineState->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kBasic, ShaderCompiler::ShaderType::kPS));
		pipelineState->SetBlendState(blendDesc[index]);
		pipelineState->SetRasterizerState(rasterizerDesc);
		pipelineState->SetDepthStencilState(depthStencilDesc);
		pipelineState->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		pipelineState->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		pipelineState->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		pipelineState->Finalize();*/

		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType] = new PipelineState(device, graphicsPipelines_[PipelineType::kSkinning]->rooSignature_);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetInputLayout(inputLayoutDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kSkinning, ShaderCompiler::ShaderType::kVS));
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kBasic, ShaderCompiler::ShaderType::kPS));
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetBlendState(blendDesc[blendModeType]);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetRasterizerState(rasterizerDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetDepthStencilState(depthStencilDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->Finalize();
	}

}

void GraphicsPipelineManager::SetCommandList(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendModeType blendModeType)
{
	commandList->SetGraphicsRootSignature(graphicsPipelines_[pipelineType]->rooSignature_->GetSignature());
	commandList->SetPipelineState(graphicsPipelines_[pipelineType]->pso_[blendModeType]->GetPipelineStateObject());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

GraphicsPipelineManager* GraphicsPipelineManager::GetInstance()
{
	static GraphicsPipelineManager instance;
	return &instance;
}
