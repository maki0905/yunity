#include "GraphicsPipelineManager.h"

#include "Device.h"
#include "ShaderCompiler.h"
#include "RootBindingsCommon.h"

void yunity::GraphicsPipelineManager::Initialize()
{
	graphicsCommon_ = GraphicsCommon::GetInstance();
	for (uint32_t pipelineType = 0; pipelineType < PipelineType::kCount; pipelineType++) {
		graphicsPipelines_[pipelineType] = std::make_unique<GraphicsPipeline>();
	}

	CreateObject3d();
	CreateParticle();
	CreateLine();
	CreatePrimitive();
	CreateSkinning();
	CreateShadowMap();
	CreateObject3dShadowMap();

}

void yunity::GraphicsPipelineManager::SetCommandList(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendModeType blendModeType)
{
	commandList->SetGraphicsRootSignature(graphicsPipelines_[pipelineType]->rooSignature_->GetSignature());
	commandList->SetPipelineState(graphicsPipelines_[pipelineType]->pso_[blendModeType]->GetPipelineStateObject());
}

void yunity::GraphicsPipelineManager::CreateObject3d()
{
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_ = std::make_unique<RootSignature>(Device::GetInstance()->GetDevice(), static_cast<UINT>(Object3dRootBindings::kCount), 1);
	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kViewProjection)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kRootNode)).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kMaterial)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kLight)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kCamera)).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kPointLight)).InitializeAsConstantBuffer(3, D3D12_SHADER_VISIBILITY_PIXEL);
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
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType] = std::make_unique<PipelineState>(Device::GetInstance()->GetDevice(), graphicsPipelines_[PipelineType::kObject3d]->rooSignature_.get());
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetInputLayout(inputLayoutDesc);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Object3d", ShaderCompiler::ShaderType::kVS));
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("Object3d", ShaderCompiler::ShaderType::kPS));
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetBlendState(graphicsCommon_->blendDescs[blendModeType]);
		//graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetBlendState(blendDesc);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetRasterizerState(rasterizerDesc);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetDepthStencilState(depthStencilDesc);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType]->Finalize();
	}
}

void yunity::GraphicsPipelineManager::CreateParticle()
{
	graphicsPipelines_[PipelineType::kParticle]->rooSignature_ = std::make_unique<RootSignature>(Device::GetInstance()->GetDevice(), static_cast<int>(ParticleRootBindings::kCount), 1);

	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kParticle]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	graphicsPipelines_[PipelineType::kParticle]->rooSignature_->GetParameter(static_cast<size_t>(ParticleRootBindings::kWorldTransform)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kParticle]->rooSignature_->GetParameter(static_cast<size_t>(ParticleRootBindings::kViewProjection)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kParticle]->rooSignature_->GetParameter(static_cast<size_t>(ParticleRootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kParticle]->rooSignature_->GetParameter(static_cast<size_t>(ParticleRootBindings::kMaterial)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);

	graphicsPipelines_[PipelineType::kParticle]->rooSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "TEXCOORD", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "NORMAL", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc = GraphicsPipelineManager::graphicsCommon_->RasterizerDefault;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = GraphicsPipelineManager::graphicsCommon_->DepthStateReadWrite;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	for (uint32_t blendModeType = 0; blendModeType < BlendModeType::kBlendCount; blendModeType++) {
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType] = std::make_unique<PipelineState>(Device::GetInstance()->GetDevice(), graphicsPipelines_[PipelineType::kParticle]->rooSignature_.get());
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetInputLayout(inputLayoutDesc);
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Particle", ShaderCompiler::ShaderType::kVS));
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("Particle", ShaderCompiler::ShaderType::kPS));
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetBlendState(graphicsCommon_->blendDescs[blendModeType]);
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetRasterizerState(rasterizerDesc);
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetDepthStencilState(depthStencilDesc);
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType]->Finalize();
	}
}

void yunity::GraphicsPipelineManager::CreateLine()
{
	graphicsPipelines_[PipelineType::kLine]->rooSignature_ = std::make_unique<RootSignature>(Device::GetInstance()->GetDevice(), static_cast<int>(LineRootBindings::kCount), 1);
	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kLine]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	graphicsPipelines_[PipelineType::kLine]->rooSignature_->GetParameter(static_cast<size_t>(LineRootBindings::kViewProjection)).InitializeAsConstantBuffer(static_cast<int>(LineRootBindings::kViewProjection));

	graphicsPipelines_[PipelineType::kLine]->rooSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "COLOR", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc = graphicsCommon_->RasterizerDefault;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = graphicsCommon_->DepthStateReadWrite;

	graphicsPipelines_[PipelineType::kLine]->pso_[0] = std::make_unique<PipelineState>(Device::GetInstance()->GetDevice(), graphicsPipelines_[PipelineType::kLine]->rooSignature_.get());
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetInputLayout(inputLayoutDesc);
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Line", ShaderCompiler::ShaderType::kVS));
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("Line", ShaderCompiler::ShaderType::kPS));
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetBlendState(graphicsCommon_->blendDescs[1]);
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetRasterizerState(rasterizerDesc);
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetDepthStencilState(depthStencilDesc);
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
	graphicsPipelines_[PipelineType::kLine]->pso_[0]->Finalize();
}

void yunity::GraphicsPipelineManager::CreatePrimitive()
{
	graphicsPipelines_[PipelineType::kPrimitive]->rooSignature_ = std::make_unique<RootSignature>(Device::GetInstance()->GetDevice(), static_cast<int>(PrimitiveRootBindings::kCount), 1);
	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kPrimitive]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	//rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kPrimitive]->rooSignature_->GetParameter(static_cast<size_t>(PrimitiveRootBindings::kWorldTransform)).InitializeAsConstantBuffer(static_cast<int>(PrimitiveRootBindings::kWorldTransform));
	graphicsPipelines_[PipelineType::kPrimitive]->rooSignature_->GetParameter(static_cast<size_t>(PrimitiveRootBindings::kViewProjection)).InitializeAsConstantBuffer(static_cast<int>(PrimitiveRootBindings::kViewProjection));

	graphicsPipelines_[PipelineType::kPrimitive]->rooSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "COLOR", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc = graphicsCommon_->RasterizerDefault;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = graphicsCommon_->DepthStateReadWrite;

	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0] = std::make_unique<PipelineState>(Device::GetInstance()->GetDevice(), graphicsPipelines_[PipelineType::kPrimitive]->rooSignature_.get());
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetInputLayout(inputLayoutDesc);
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Primitive", ShaderCompiler::ShaderType::kVS));
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("Primitive", ShaderCompiler::ShaderType::kPS));
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetBlendState(graphicsCommon_->blendDescs[1]);
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetRasterizerState(rasterizerDesc);
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetDepthStencilState(depthStencilDesc);
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0]->Finalize();

}

void yunity::GraphicsPipelineManager::CreateSkinning()
{
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_ = std::make_unique<RootSignature>(Device::GetInstance()->GetDevice(), static_cast<UINT>(SkinningRootBindings::kCount), 1);
	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kViewProjection)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kRootNode)).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kMaterial)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kLight)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kCamera)).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kPointLight)).InitializeAsConstantBuffer(3, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kMatrixPalette)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_->GetParameter(static_cast<UINT>(SkinningRootBindings::kEnvironmentMap)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, D3D12_SHADER_VISIBILITY_PIXEL);
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
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType] = std::make_unique<PipelineState>(Device::GetInstance()->GetDevice(), graphicsPipelines_[PipelineType::kSkinning]->rooSignature_.get());
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetInputLayout(inputLayoutDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Skinning", ShaderCompiler::ShaderType::kVS));
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("Object3d", ShaderCompiler::ShaderType::kPS));
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetBlendState(graphicsCommon_->blendDescs[blendModeType]);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetRasterizerState(rasterizerDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetDepthStencilState(depthStencilDesc);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType]->Finalize();
	}
}

void yunity::GraphicsPipelineManager::CreateShadowMap()
{
	graphicsPipelines_[PipelineType::kShadowMap]->rooSignature_ = std::make_unique<RootSignature>(Device::GetInstance()->GetDevice(), static_cast<UINT>(ShadowMapBindings::kCount), 1);
	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kShadowMap]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	graphicsPipelines_[PipelineType::kShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(ShadowMapBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(ShadowMapBindings::kViewProjection)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kShadowMap]->rooSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC object3DInputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "TEXCOORD", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "NORMAL", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	inputLayoutDesc.pInputElementDescs = object3DInputElementDescs;
	inputLayoutDesc.NumElements = _countof(object3DInputElementDescs);

	D3D12_RASTERIZER_DESC rasterizerDesc = GraphicsPipelineManager::graphicsCommon_->RasterizerDefault;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = GraphicsPipelineManager::graphicsCommon_->DepthStateReadWrite;
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable = FALSE;

	for (uint32_t blendModeType = 0; blendModeType < BlendModeType::kBlendCount; blendModeType++) {
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType] = std::make_unique<PipelineState>(
			Device::GetInstance()->GetDevice(), graphicsPipelines_[PipelineType::kShadowMap]->rooSignature_.get());
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetInputLayout(inputLayoutDesc);
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetShader(
			PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("ShadowMap", ShaderCompiler::ShaderType::kVS));
		//graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kPS, nullptr); // ピクセルシェーダなし
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetBlendState(graphicsCommon_->blendDescs[blendModeType]);
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetRasterizerState(rasterizerDesc);
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetDepthStencilState(depthStencilDesc);
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		graphicsPipelines_[PipelineType::kShadowMap]->pso_[blendModeType]->Finalize();
	}
}

void yunity::GraphicsPipelineManager::CreateObject3dShadowMap()
{
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_ = std::make_unique<RootSignature>(Device::GetInstance()->GetDevice(), static_cast<UINT>(Object3dShadowMapRootBindings::kCount), 2);

	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	staticSamplers.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	staticSamplers.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	staticSamplers.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	staticSamplers.MipLODBias = 0.0f;
	staticSamplers.MaxAnisotropy = 16;
	staticSamplers.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	staticSamplers.MinLOD = 0.0f;
	staticSamplers.MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers.ShaderRegister = 1;
	staticSamplers.RegisterSpace = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->InitializeStaticSampler(1, staticSamplers, D3D12_SHADER_VISIBILITY_ALL);

	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kViewProjection)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kRootNode)).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kLightViewProjection)).InitializeAsConstantBuffer(3, D3D12_SHADER_VISIBILITY_VERTEX);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kShadowMap)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kMaterial)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kLight)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kCamera)).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->GetParameter(static_cast<UINT>(Object3dShadowMapRootBindings::kPointLight)).InitializeAsConstantBuffer(3, D3D12_SHADER_VISIBILITY_PIXEL);
	graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

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
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType] = std::make_unique<PipelineState>(Device::GetInstance()->GetDevice(), graphicsPipelines_[PipelineType::kObject3dShadowMap]->rooSignature_.get());
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetInputLayout(inputLayoutDesc);
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Object3dShadowMap", ShaderCompiler::ShaderType::kVS));
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("Object3dShadowMap", ShaderCompiler::ShaderType::kPS));
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetBlendState(graphicsCommon_->blendDescs[blendModeType]);
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetRasterizerState(rasterizerDesc);
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetDepthStencilState(depthStencilDesc);
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		graphicsPipelines_[PipelineType::kObject3dShadowMap]->pso_[blendModeType]->Finalize();
	}
}

yunity::GraphicsPipelineManager* yunity::GraphicsPipelineManager::GetInstance()
{
	static GraphicsPipelineManager instance;
	return &instance;
}
