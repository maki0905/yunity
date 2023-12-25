#include "PrimitiveDrawer.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "Device.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "ShaderCompiler.h"


ID3D12Device* PrimitiveDrawer::device_ = nullptr;
ID3D12GraphicsCommandList* PrimitiveDrawer::commandList_ = nullptr;
RootSignature* PrimitiveDrawer::rootSignature_ = nullptr;
PipelineState* PrimitiveDrawer::pipelineState_ = nullptr;

void PrimitiveDrawer::StaticInitialize()
{
	device_ = Device::GetInstance()->GetDevice();

	InitializeGraphicsPipeline();
}

void PrimitiveDrawer::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(commandList_ == nullptr);

	commandList_ = commandList;
}

void PrimitiveDrawer::PostDraw()
{
	commandList_ = nullptr;
}

PrimitiveDrawer* PrimitiveDrawer::Create(Type type)
{
	PrimitiveDrawer* primitiveDrawer = new PrimitiveDrawer();
	switch (type)
	{
	case PrimitiveDrawer::Type::kBox:
		primitiveDrawer->CreateBox();
		break;
	case PrimitiveDrawer::Type::kSphere:
		primitiveDrawer->CreateSphere();
		break;
	default:
		break;
	}
	primitiveDrawer->CreateMesh();
	return primitiveDrawer;
}

void PrimitiveDrawer::InitializeGraphicsPipeline()
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

	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kWorldTransform)).InitializeAsConstantBuffer(0);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kViewProjection)).InitializeAsConstantBuffer(1);


	rootSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	pipelineState_ = new PipelineState(device_, rootSignature_);

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{.SemanticName = "POSITION", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT},
		{.SemanticName = "COLOR", .SemanticIndex = 0, .Format = DXGI_FORMAT_R32G32B32A32_FLOAT, .AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT}
	};

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendState
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	/*blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;*/


	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	pipelineState_->SetInputLayout(inputLayoutDesc);
	pipelineState_->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kLine, ShaderCompiler::ShaderType::kVS));
	pipelineState_->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kLine, ShaderCompiler::ShaderType::kPS));
	pipelineState_->SetBlendState(blendDesc);
	pipelineState_->SetRasterizerState(rasterizerDesc);
	pipelineState_->SetDepthStencilState(depthStencilDesc);
	pipelineState_->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pipelineState_->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
	pipelineState_->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
	pipelineState_->Finalize();



}

void PrimitiveDrawer::Draw(const WorldTransform& worldTransform, const Camera& camera)
{
	assert(device_);
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	commandList_->SetPipelineState(pipelineState_->GetPipelineStateObject());
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

	commandList_->IASetIndexBuffer(&indexBufferView_);

	// CBVをセット(ワールド行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kViewProjection), camera.constBuff_->GetGPUVirtualAddress());

	commandList_->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);

	//commandList_->DrawIndexedInstanced(16 * 16 * 6, 1, 0, 0, 0);
	//commandList_->DrawIndexedInstanced(24, 1, 0, 0, 0);
}

void PrimitiveDrawer::CreateMesh()
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

void PrimitiveDrawer::CreateSphere()
{
	

	const uint32_t kSubdivision = 16;
	

	// 経度分割1つ分の角度 φ
	const float kLonEvery = float(M_PI) * 2.0f / float(kSubdivision);
	// 緯度分割1つ分の角度 θ
	const float kLatEvery = float(M_PI) / float(kSubdivision);
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;// θ
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 4;
			float lon = lonIndex * kLonEvery;// φ

			VertexData vertexData[4];
			// 頂点にデータを入力する。基準点a
			vertexData[0].position.x = cos(lat) * cos(lon);
			vertexData[0].position.y = sin(lat);
			vertexData[0].position.z = cos(lat) * sin(lon);
			vertexData[0].position.w = 1.0f;
			vertexData[0].color = { 1, 1, 1, 1 };
			// b
			vertexData[1].position.x = cos(lat + kLatEvery) * cos(lon);
			vertexData[1].position.y = sin(lat + kLatEvery);
			vertexData[1].position.z = cos(lat + kLatEvery) * sin(lon);
			vertexData[1].position.w = 1.0f;
			vertexData[1].color = { 1, 1, 1, 1 };
			// c
			vertexData[2].position.x = cos(lat) * cos(lon + kLonEvery);
			vertexData[2].position.y = sin(lat);
			vertexData[2].position.z = cos(lat) * sin(lon + kLonEvery);
			vertexData[2].position.w = 1.0f;
			vertexData[2].color = { 1, 1, 1, 1 };
			// d
			vertexData[3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			vertexData[3].position.y = sin(lat + kLatEvery);
			vertexData[3].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			vertexData[3].position.w = 1.0f;
			vertexData[3].color = { 1, 1, 1, 1 };

			for (uint32_t i = 0; i < 4; i++) {
				vertices_.push_back(vertexData[i]);
			}

			//// 頂点にデータを入力する。基準点a
			//vertexData_[start].position.x = cos(lat) * cos(lon);
			//vertexData_[start].position.y = sin(lat);
			//vertexData_[start].position.z = cos(lat) * sin(lon);
			//vertexData_[start].position.w = 1.0f;
			//vertexData_[start].color = { 1, 1, 1, 1 };
			//// b
			//vertexData_[start + 1].position.x = cos(lat + kLatEvery) * cos(lon);
			//vertexData_[start + 1].position.y = sin(lat + kLatEvery);
			//vertexData_[start + 1].position.z = cos(lat + kLatEvery) * sin(lon);
			//vertexData_[start + 1].position.w = 1.0f;
			//vertexData_[start + 1].color = { 1, 1, 1, 1 };
			//// c
			//vertexData_[start + 2].position.x = cos(lat) * cos(lon + kLonEvery);
			//vertexData_[start + 2].position.y = sin(lat);
			//vertexData_[start + 2].position.z = cos(lat) * sin(lon + kLonEvery);
			//vertexData_[start + 2].position.w = 1.0f;
			//vertexData_[start + 2].color = { 1, 1, 1, 1 };
			//// d
			//vertexData_[start + 3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			//vertexData_[start + 3].position.y = sin(lat + kLatEvery);
			//vertexData_[start + 3].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			//vertexData_[start + 3].position.w = 1.0f;
			//vertexData_[start + 3].color = { 1, 1, 1, 1 };
		}

	}

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			uint32_t i = (latIndex * kSubdivision + lonIndex) * 4;

			uint32_t indexData[6];
			indexData[0] = i;
			indexData[1] = i + 1;
			indexData[2] = i + 2;
			indexData[3] = i + 1;
			indexData[4] = i + 3;
			indexData[5] = i + 2;

			for (uint32_t i = 0; i < 6; i++) {
				indices_.push_back(indexData[i]);
			}

		}

	}

	/*for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			uint32_t i = (latIndex * kSubdivision + lonIndex) * 4;
			indexData_[start] = i;
			indexData_[start + 1] = i + 1;
			indexData_[start + 2] = i + 2;
			indexData_[start + 3] = i + 1;
			indexData_[start + 4] = i + 3;
			indexData_[start + 5] = i + 2;

		}

	}*/
}

void PrimitiveDrawer::CreateBox()
{
	//// 頂点リソース
	//vertexResource_ = CreateBufferResource(sizeof(VertexData) * 8);
	//// 頂点バッファビュー
	//vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	//vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * 8); // 使用するリソースのサイズは頂点サイズ
	//vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	//// 頂点リソースにデータを書き込む
	//vertexResource_->Map(0, nullptr, (void**)&vertexData_); // 書き込ためのアドレスを取得

	//indexBuff_ = CreateBufferResource(sizeof(uint32_t) * 36);
	//indexBufferView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	//indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 36);
	//indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//indexBuff_->Map(0, nullptr, (void**)&indexData_);

	VertexData vertexData[8];
	vertexData[0] = { Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData[1] = { Vector4(-0.5f, 0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData[2] = { Vector4(0.5f, 0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData[3] = { Vector4(0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData[4] = { Vector4(-0.5f, -0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData[5] = { Vector4(-0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData[6] = { Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData[7] = { Vector4(0.5f, -0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };

	for (int32_t i = 0; i < 8; i++) {
		vertices_.push_back(vertexData[i]);
	}

	uint32_t indexData[24];

	indexData[0] = 0; indexData[1] = 1;
	indexData[2] = 1; indexData[3] = 2;
	indexData[4] = 2; indexData[5] = 3;
	indexData[6] = 3; indexData[7] = 0;

	indexData[8] = 4;  indexData[9] = 5;
	indexData[10] = 5; indexData[11] = 6;
	indexData[12] = 6; indexData[13] = 7;
	indexData[14] = 7; indexData[15] = 4;

	indexData[16] = 0; indexData[17] = 4;
	indexData[18] = 1; indexData[19] = 5;
	indexData[20] = 2; indexData[21] = 6;
	indexData[22] = 3; indexData[23] = 7;

	for (int32_t i = 0; i < 24; i++) {
		indices_.push_back(indexData[i]);
	}

	/*vertexData_[0] = { Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData_[1] = { Vector4(-0.5f, 0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData_[2] = { Vector4(0.5f, 0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData_[3] = { Vector4(0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData_[4] = { Vector4(-0.5f, -0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData_[5] = { Vector4(-0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData_[6] = { Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData_[7] = { Vector4(0.5f, -0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };

	indexData_[0] = 0; indexData_[1] = 1; 
	indexData_[2] = 1; indexData_[3] = 2; 
	indexData_[4] = 2; indexData_[5] = 3;
	indexData_[6] = 3; indexData_[7] = 0; 

	indexData_[8] = 4;  indexData_[9] = 5; 
	indexData_[10] = 5; indexData_[11] = 6;
	indexData_[12] = 6; indexData_[13] = 7; 
	indexData_[14] = 7; indexData_[15] = 4; 

	indexData_[16] = 0; indexData_[17] = 4;
	indexData_[18] = 1; indexData_[19] = 5; 
	indexData_[20] = 2; indexData_[21] = 6; 
	indexData_[22] = 3; indexData_[23] = 7;*/

}

ID3D12Resource* PrimitiveDrawer::CreateBufferResource(size_t sizeInBytes)
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

