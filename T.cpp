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


	PSO_ = new PipelineState(device_.Get(), rootSignature_);

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITIONT";
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

	PSO_->SetShader(PipelineState::ShaderType::kVS, Shader::GetInstance()->Get(Shader::Name::BasicVS));
	PSO_->SetShader(PipelineState::ShaderType::kPS, Shader::GetInstance()->Get(Shader::Name::BasicPS));

	PSO_->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_UNKNOWN);
	PSO_->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	PSO_->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);

	PSO_->Finalize();

	CreateMesh();

}

void T::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(T::commandList_ == nullptr);

	commandList_ = commandList;

	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	commandList_->SetPipelineState(PSO_->GetPipelineStateObject());
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList_->DrawInstanced(3, 1, 0, 0);

}

void T::PostDraw()
{
	commandList_ = nullptr;
}

//void T::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHandle)
//{
//	assert(device_);
//	assert(commandList_);
//	assert(worldTransform.constBuff_.Get());
//
//	// 頂点バッファの設定
//	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
//
//	// CBVをセット(ワールド行列)
//	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());
//
//	// CBVをセット(ビュープロジェクション行列)
//	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kViewProjection), viewProjection.constBuff_->GetGPUVirtualAddress());
//
//	// SRVをセット
//	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_.Get(), static_cast<UINT>(RoomParameter::kTexture), textureHandle);
//
//	commandList_->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
//
//}

//void T::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection)
//{
//	assert(device_);
//	assert(commandList_);
//	assert(worldTransform.constBuff_.Get());
//
//	// 頂点バッファの設定
//	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
//
//	// CBVをセット(ワールド行列)
//	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());
//
//	// CBVをセット(ビュープロジェクション行列)
//	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kViewProjection), viewProjection.constBuff_->GetGPUVirtualAddress());
//
//	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kMaterial), materialResource_->GetGPUVirtualAddress());
//
//	// CBVをセット(ビュープロジェクション行列)
//	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kLight), directionalLightResource_->GetGPUVirtualAddress());
//
//
//	// SRVをセット
//	if (textureHandle_ != TextureManager::Load(modelData.material.textureFilePath)) {
//		textureHandle_ = TextureManager::Load(modelData.material.textureFilePath);
//	}
//	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_.Get(), static_cast<UINT>(RoomParameter::kTexture), textureHandle_);
//
//	commandList_->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
//}

//void T::SetMaterial(const Vector4& color)
//{
//	color;
//}

void T::CreateMesh()
{	
	HRESULT result = S_FALSE;
	vertexResource_ = CreateBufferResource(sizeof(Vector4) * 3);
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(Vector4) * 3);
	vertexBufferView_.StrideInBytes = UINT(sizeof(Vector4));

	vertexData_ = {
		{-0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.0f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f }
	};

	Vector4* vertexData = nullptr;
	
	result = vertexResource_->Map(0, nullptr, (void**)&vertexData);
	if (SUCCEEDED(result)) {
		std::copy(vertexData_.begin(), vertexData_.end(), vertexData);
		vertexResource_->Unmap(0, nullptr);
	}

}

//void T::LoadObjFile(const std::string& filename)
//{
//	filename;
//}

//T::MaterialData T::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
//{
//	filename;
//	directoryPath;
//	return MaterialData();
//}

//void T::InitializeDirectionalLight()
//{
//}

//void T::InitializeMaterial()
//{
//}

ID3D12Resource* T::CreateBufferResource(size_t sizeInBytes)
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
