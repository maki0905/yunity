#include "SkyBox.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "Device.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "ShaderCompiler.h"
#include "TextureManager.h"


ID3D12Device* SkyBox::device_ = nullptr;
ID3D12GraphicsCommandList* SkyBox::commandList_ = nullptr;
RootSignature* SkyBox::rootSignature_ = nullptr;
PipelineState* SkyBox::pipelineState_ = nullptr;

void SkyBox::StaticInitialize()
{
	device_ = Device::GetInstance()->GetDevice();

	InitializeGraphicsPipeline();
}

void SkyBox::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(commandList_ == nullptr);

	commandList_ = commandList;
}

void SkyBox::PostDraw()
{
	commandList_ = nullptr;
}

SkyBox* SkyBox::Create()
{
	SkyBox* skyBox = new SkyBox();
	skyBox->CreateBox();
	skyBox->CreateMesh();
	
	return skyBox;
}

void SkyBox::InitializeGraphicsPipeline()
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

	rootSignature_->GetParameter(static_cast<UINT>(RootBindings::kWorldTransform)).InitializeAsConstantBuffer(0);
	rootSignature_->GetParameter(static_cast<UINT>(RootBindings::kViewProjection)).InitializeAsConstantBuffer(1);
	rootSignature_->GetParameter(static_cast<UINT>(RootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);


	rootSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	pipelineState_ = new PipelineState(device_, rootSignature_);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "TEXCOORD", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "NORMAL", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32_FLOAT,.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
	};

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendState
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true; // Depthの機能を有効化する
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; // 全ピクセルがz=1に出力されるので、わざわざ書き込む必要がない
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; // 比較関数はLessEqual。つまり、近ければ描画される

	pipelineState_->SetInputLayout(inputLayoutDesc);
	pipelineState_->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Skybox", ShaderCompiler::ShaderType::kVS));
	pipelineState_->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("Skybox", ShaderCompiler::ShaderType::kPS));
	pipelineState_->SetBlendState(blendDesc);
	pipelineState_->SetRasterizerState(rasterizerDesc);
	pipelineState_->SetDepthStencilState(depthStencilDesc);
	pipelineState_->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pipelineState_->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pipelineState_->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
	pipelineState_->Finalize();
}

void SkyBox::Draw(const WorldTransform& worldTransform)
{
	assert(device_);
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	commandList_->SetPipelineState(pipelineState_->GetPipelineStateObject());
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

	commandList_->IASetIndexBuffer(&indexBufferView_);

	// CBVをセット(ワールド行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());

	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(RootBindings::kTexture), textureHandle_);

	commandList_->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);

	//commandList_->DrawIndexedInstanced(16 * 16 * 6, 1, 0, 0, 0);
	//commandList_->DrawIndexedInstanced(24, 1, 0, 0, 0);
}

void SkyBox::SetTexture(const std::string& textureName)
{
	textureHandle_ = TextureManager::GetInstance()->Load(textureName);
}

void SkyBox::CreateMesh()
{
	// 頂点リソース
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * vertices_.size()/** 16 * 16 * 4*/);
	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * vertices_.size()/* * 16 * 16 * 4*/); // 使用するリソースのサイズは頂点サイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	//VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, (void**)&vertexData_/*vertexData_*/); // 書き込ためのアドレスを取得
	std::memcpy(vertexData_, vertices_.data(), sizeof(VertexData) * vertices_.size());

	indexBuff_ = CreateBufferResource(sizeof(uint32_t) * indices_.size()/** 16 * 16 * 6*/);
	indexBufferView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indices_.size()/** 16 * 16 * 6*/);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//uint32_t* indexData = nullptr;
	indexBuff_->Map(0, nullptr, (void**)&indexData_/*indexData_*/);
	std::memcpy(indexData_, indices_.data(), sizeof(uint32_t) * indices_.size());

}

void SkyBox::CreateBox()
{
	VertexData vertexData[24];

	// 右面。描画インデックスは[0, 1, 2][2, 1, 3]で内側を向く
	vertexData[0] = {1.0f, 1.0f, 1.0f, 1.0f};
	vertexData[1] = {1.0f, 1.0f, -1.0f, 1.0f};
	vertexData[2] = {1.0f, -1.0f, 1.0f, 1.0f};
	vertexData[3] = {1.0f, -1.0f, -1.0f, 1.0f};
	// 左面。描画インデックスは[4, 5, 6][6, 5, 7]で内側を向く
	vertexData[4] = {-1.0f, 1.0f, -1.0f, 1.0f};
	vertexData[5] = {-1.0f, 1.0f, 1.0f, 1.0f};
	vertexData[6] = {-1.0f, -1.0f, -1.0f, 1.0f};
	vertexData[7] = {-1.0f, -1.0f, 1.0f, 1.0f};
	// 前面。描画インデックスは[8, 9, 10][10, 9, 11]で内側を向く
	vertexData[8] = {-1.0f, 1.0f, 1.0f, 1.0f};
	vertexData[9] = {1.0f, 1.0f, 1.0f, 1.0f};
	vertexData[10] ={-1.0f, -1.0f, 1.0f, 1.0f};
	vertexData[11] ={1.0f, -1.0f, 1.0f, 1.0f};
	// 後面。描画インデックスは[12, 13, 14][14, 13, 15]で内側を向く
	vertexData[12] = {-1.0f, 1.0f, -1.0f, 1.0f};
	vertexData[13] = {1.0f, 1.0f, -1.0f, 1.0f};
	vertexData[14] = {-1.0f, -1.0f, -1.0f, 1.0f};
	vertexData[15] = {1.0f, -1.0f, -1.0f, 1.0f};
	// 上面。描画インデックスは[16, 17, 18][18, 17, 19]で内側を向く
	vertexData[16] = {-1.0f, 1.0f, 1.0f, 1.0f};
	vertexData[17] = {1.0f, 1.0f, 1.0f, 1.0f};
	vertexData[18] = {-1.0f, 1.0f, -1.0f, 1.0f};
	vertexData[19] = {1.0f, 1.0f, -1.0f, 1.0f};
	// 下面。描画インデックスは[20, 21, 22][22, 21, 23]で内側を向く
	vertexData[20] = {-1.0f, -1.0f, 1.0f, 1.0f};
	vertexData[21] = {1.0f, -1.0f, 1.0f, 1.0f};
	vertexData[22] = {-1.0f, -1.0f, -1.0f, 1.0f};
	vertexData[23] = {1.0f, -1.0f, -1.0f, 1.0f};

	for (int32_t i = 0; i < 24; i++) {
		vertices_.push_back(vertexData[i]);
	}

	uint32_t indexData[36];

	// 左面
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 2; indexData[4] = 1; indexData[5] = 3;
	// 右面
	indexData[6] = 4; indexData[7] = 5; indexData[8] = 6;
	indexData[9] = 6; indexData[10] = 5; indexData[11] = 7;
	// 前面
	indexData[12] = 8; indexData[13] = 9; indexData[14] = 10;
	indexData[15] = 10; indexData[16] = 9; indexData[17] = 11;
	// 後面
	indexData[18] = 12; indexData[19] = 13; indexData[20] = 14;
	indexData[21] = 14; indexData[22] = 13; indexData[23] = 15;
	// 上面
	indexData[24] = 16; indexData[25] = 17; indexData[26] = 18;
	indexData[27] = 18; indexData[28] = 17; indexData[29] = 19;
	// 下面
	indexData[30] = 20; indexData[31] = 21; indexData[32] = 22;
	indexData[33] = 22; indexData[34] = 21; indexData[35] = 23;

	for (int32_t i = 0; i < 36; i++) {
		indices_.push_back(indexData[i]);
	}

}

ID3D12Resource* SkyBox::CreateBufferResource(size_t sizeInBytes)
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

