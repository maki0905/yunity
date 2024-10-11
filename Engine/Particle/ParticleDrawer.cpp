#include "ParticleDrawer.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "Device.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "ShaderCompiler.h"
#include "DirectXCore.h"

ID3D12Device* ParticleDrawer::device_ = nullptr;
ID3D12GraphicsCommandList* ParticleDrawer::commandList_ = nullptr;
RootSignature* ParticleDrawer::rootSignature_ = nullptr;
PipelineState* ParticleDrawer::pipelineState_ = nullptr;


void ParticleDrawer::StaticInitialize()
{
	device_ = Device::GetInstance()->GetDevice();

	InitializeGraphicsPipeline();
}

void ParticleDrawer::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(commandList_ == nullptr);

	commandList_ = commandList;
	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	commandList_->SetPipelineState(pipelineState_->GetPipelineStateObject());
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ParticleDrawer::PostDraw()
{
	commandList_ = nullptr;
}

ParticleDrawer* ParticleDrawer::Create(const std::string& modelname)
{
	ParticleDrawer* particleDrawer = new ParticleDrawer();
	particleDrawer->Initialize(modelname);
	return particleDrawer;
}

void ParticleDrawer::InitializeGraphicsPipeline()
{
	rootSignature_ = new RootSignature(device_, static_cast<int>(RootBindings::kCount), 1);

	D3D12_STATIC_SAMPLER_DESC staticSamplers = {};
	staticSamplers.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
	staticSamplers.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers.MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers.ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderを使う
	rootSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	//rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kWorldTransform)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kViewProjection)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kMaterial)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);

	rootSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	pipelineState_ = new PipelineState(device_, rootSignature_);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "TEXCOORD", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "NORMAL", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendState
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みを行わない
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	pipelineState_->SetInputLayout(inputLayoutDesc);
	pipelineState_->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Particle", ShaderCompiler::ShaderType::kVS));
	pipelineState_->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("Particle", ShaderCompiler::ShaderType::kPS));
	pipelineState_->SetBlendState(blendDesc);
	pipelineState_->SetRasterizerState(rasterizerDesc);
	pipelineState_->SetDepthStencilState(depthStencilDesc);
	pipelineState_->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pipelineState_->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pipelineState_->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
	pipelineState_->Finalize();
}


void ParticleDrawer::Initialize(const std::string& modelname)
{
	CreateMesh();
	textureHandle_ = TextureManager::Load(modelname);
	CreateSRV();
	InitializeMaterial();
}

void ParticleDrawer::Draw(/*WorldTransform* worldTransform,*/std::list<Particle*> particles)
{
	assert(device_);
	assert(commandList_);

	uint32_t numInstance = 0;
	
	//for (uint32_t index = 0; index < kNumInstance; index++) {
	//	instancingData_[index].matWorld_ = worldTransform[index].matWorld_;
	//	//instancingData_[index].constBuff_ = worldTransform[index].constBuff_;
	//}
	//for (uint32_t index = 0; index < kNumMaxInstance; index++) {
	//	//instancingData_[index].world = worldTransform[index].matWorld_;
	//	++numInstance;
	//	/*if (numInstance >= kNumMaxInstance) {
	//		break;
	//	}*/
	//}

	for (uint32_t index = 0; Particle* particle : particles) {
		instancingData_[index] = particle->particleForCPU;
		//instancingData_[index] = particle->color;
		index++;
		++numInstance;

		if (numInstance >= kNumMaxInstance) {
			break;
		}
	}



	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { /*srvHeap_.Get()*/ srvHeap_->GetHeapPointer() };
	commandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// CBVをセット(ワールド行列)
	//commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(RootBindings::kWorldTransform), instancingSrvHandleGPU_);
	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kMaterial), materialResource_->GetGPUVirtualAddress());


	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(RootBindings::kTexture), textureHandle_);

	commandList_->DrawInstanced(UINT(modelData.vertices.size()), numInstance, 0, 0);
}

void ParticleDrawer::SetMaterial(const Vector4& color)
{
	materialData_->color = color;
}

void ParticleDrawer::CreateMesh()
{

	modelData.vertices.push_back({ .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左上
	modelData.vertices.push_back({ .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右上
	modelData.vertices.push_back({ .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左下
	modelData.vertices.push_back({ .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左下
	modelData.vertices.push_back({ .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右上
	modelData.vertices.push_back({ .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右下

	// 頂点リソース
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size()); // 使用するリソースのサイズは頂点サイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_->Map(0, nullptr, (void**)&vertexData_); // 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size()); // 頂点データをリソースにコピー
}



void ParticleDrawer::CreateSRV()
{
	//instancingResource_ = CreateBufferResource(sizeof(WorldTransform) * kNumInstance);
	instancingResource_ = CreateBufferResource(sizeof(ParticleForCPU) * kNumMaxInstance);
	//instancingResource_->Map(0, nullptr, (void**)&instancingData_);
	instancingResource_->Map(0, nullptr, (void**)&instancingData_);

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].world = MakeIdentity4x4();
		//instancingDatas_[index].color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	}

	srvHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV);
	DescriptorHandle srvHandle = srvHeap_->Alloc();


	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	//instancingSrvDesc.Buffer.StructureByteStride = sizeof(WorldTransform);
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForCPU);
	/*instancingSrvHandleCPU_ = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	instancingSrvHandleGPU_ = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();*/
	instancingSrvHandleCPU_ = srvHandle.GetCPUHandle();
	instancingSrvHandleGPU_ = srvHandle.GetGPUHandle();
	device_->CreateShaderResourceView(instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

}


void ParticleDrawer::InitializeMaterial()
{
	materialResource_ = CreateBufferResource(sizeof(MaterialData));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

ID3D12Resource* ParticleDrawer::CreateBufferResource(size_t sizeInBytes)
{
	HRESULT result = S_FALSE;
	// リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // uploadHeapを使う
	// リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};
	// バッファリソース
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInBytes; // リソースのサイズ
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// リソースを作る
	ID3D12Resource* resource = nullptr;
	result = device_->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));
	return resource;
}
