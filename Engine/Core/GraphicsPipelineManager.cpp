#include "GraphicsPipelineManager.h"

#include "Device.h"
#include "ShaderCompiler.h"
#include "RootBindingsCommon.h"

void GraphicsPipelineManager::Initialize()
{
	device_ = Device::GetInstance()->GetDevice();
	graphicsCommon_ = GraphicsCommon::GetInstance();

	for (uint32_t pipelineType = 0; pipelineType < PipelineType::kCount; pipelineType++) {
		graphicsPipelines_[pipelineType] = new GraphicsPipeline();
	}

	CreateObject3d();
	CreateSprite();
	CreateParticle();
	CreateLine();
	CreatePrimitive();
	CreateSkinning();

}

void GraphicsPipelineManager::SetCommandList(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendModeType blendModeType)
{
	commandList->SetGraphicsRootSignature(graphicsPipelines_[pipelineType]->rooSignature_->GetSignature());
	commandList->SetPipelineState(graphicsPipelines_[pipelineType]->pso_[blendModeType]->GetPipelineStateObject());
}

void GraphicsPipelineManager::CreateObject3d()
{
	graphicsPipelines_[PipelineType::kObject3d]->rooSignature_ = new RootSignature(device_, static_cast<UINT>(Object3dRootBindings::kCount), 1);
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
	//graphicsPipelines_[PipelineType::kObject3d]->rooSignature_->GetParameter(static_cast<UINT>(Object3dRootBindings::kEnvironmentMap)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, D3D12_SHADER_VISIBILITY_PIXEL);
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

	//// BlendState
	//D3D12_BLEND_DESC blendDesc{};
	//// すべての色要素を書き込む
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//blendDesc.RenderTarget[0].BlendEnable = TRUE;
	//blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	//blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	for (uint32_t blendModeType = 0; blendModeType < BlendModeType::kBlendCount; blendModeType++) {
		graphicsPipelines_[PipelineType::kObject3d]->pso_[blendModeType] = new PipelineState(device_, graphicsPipelines_[PipelineType::kObject3d]->rooSignature_);
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

void GraphicsPipelineManager::CreateSprite()
{
}

void GraphicsPipelineManager::CreateParticle()
{
	graphicsPipelines_[PipelineType::kParticle]->rooSignature_ = new RootSignature(device_, static_cast<int>(ParticleRootBindings::kCount), 1);

	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kParticle]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	//rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
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

	for (uint32_t blendModeType = 0; blendModeType < BlendModeType::kBlendCount; blendModeType++) {
		graphicsPipelines_[PipelineType::kParticle]->pso_[blendModeType] = new PipelineState(device_, graphicsPipelines_[PipelineType::kParticle]->rooSignature_);
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

void GraphicsPipelineManager::CreateLine()
{
	graphicsPipelines_[PipelineType::kLine]->rooSignature_ = new RootSignature(device_, static_cast<int>(LineRootBindings::kCount), 1);

	D3D12_STATIC_SAMPLER_DESC staticSamplers = graphicsCommon_->StaticSampler;
	staticSamplers.ShaderRegister = 0;
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	graphicsPipelines_[PipelineType::kLine]->rooSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	//rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
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

	graphicsPipelines_[PipelineType::kLine]->pso_[0] = new PipelineState(device_, graphicsPipelines_[PipelineType::kLine]->rooSignature_);
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

void GraphicsPipelineManager::CreatePrimitive()
{
	graphicsPipelines_[PipelineType::kPrimitive]->rooSignature_ = new RootSignature(device_, static_cast<int>(PrimitiveRootBindings::kCount), 1);

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

	graphicsPipelines_[PipelineType::kPrimitive]->pso_[0] = new PipelineState(device_, graphicsPipelines_[PipelineType::kPrimitive]->rooSignature_);
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

void GraphicsPipelineManager::CreateSkinning()
{
	graphicsPipelines_[PipelineType::kSkinning]->rooSignature_ = new RootSignature(device_, static_cast<UINT>(SkinningRootBindings::kCount), 1);
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
		graphicsPipelines_[PipelineType::kSkinning]->pso_[blendModeType] = new PipelineState(device_, graphicsPipelines_[PipelineType::kSkinning]->rooSignature_);
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

GraphicsPipelineManager* GraphicsPipelineManager::GetInstance()
{
	static GraphicsPipelineManager instance;
	return &instance;
}
