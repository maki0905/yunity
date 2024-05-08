#include "Model.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "Device.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "ShaderCompiler.h"
#include "ModelManager.h"

ID3D12Device* Model::device_ = nullptr;
ID3D12GraphicsCommandList* Model::commandList_ = nullptr;
RootSignature* Model::rootSignature_ = nullptr;
PipelineState* Model::pipelineState_ = nullptr;


void Model::StaticInitialize()
{
	device_ = Device::GetInstance()->GetDevice();

	InitializeGraphicsPipeline();
}

void Model::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(commandList_ == nullptr);

	commandList_ = commandList;
	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	commandList_->SetPipelineState(pipelineState_->GetPipelineStateObject());
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::PostDraw()
{
	commandList_ = nullptr;
}

Model* Model::Create(const std::string& fileName, const std::string format)
{
	Model* model = new Model();
	//model->SetModelData(fileName, format);
	////model->Initialize();
	return model;
}

void Model::InitializeGraphicsPipeline()
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

	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kWorldTransform)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kViewProjection)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kRootNode)).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kMaterial)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kLight)).InitializeAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kCamera)).InitializeAsConstantBuffer(2, D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kPointLight)).InitializeAsConstantBuffer(3, D3D12_SHADER_VISIBILITY_PIXEL);

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
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	
	pipelineState_->SetInputLayout(inputLayoutDesc);
	pipelineState_->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kBasic, ShaderCompiler::ShaderType::kVS));
	pipelineState_->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get(ShaderCompiler::FileName::kBasic, ShaderCompiler::ShaderType::kPS));
	pipelineState_->SetBlendState(blendDesc);
	pipelineState_->SetRasterizerState(rasterizerDesc);
	pipelineState_->SetDepthStencilState(depthStencilDesc);
	pipelineState_->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pipelineState_->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pipelineState_->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
	pipelineState_->Finalize();
}

void Model::Initialize(const ModelData& modelData, const Animation& animation)
{
	modelData_ = modelData;
	animation_ = animation;
	CreateMesh();
	InitializeDirectionalLight();
	InitializeMaterial();
	InitializeNode();
}

void Model::Draw(const WorldTransform& worldTransform, uint32_t textureHandle)
{
	assert(device_);
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	commandList_->SetPipelineState(pipelineState_->GetPipelineStateObject());
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// CBVをセット(ワールド行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());
	//commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kViewProjection), camera.GetConstBuff()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kMaterial), materialResource_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kLight), directionalLightResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kCamera), camera_->GetCameraForGPU()->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kPointLight), pointLightResource_->GetGPUVirtualAddress());

	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(RootBindings::kTexture), textureHandle);

	commandList_->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

void Model::Draw(const WorldTransform& worldTransform/*, const Camera& camera*/)
{
	assert(device_);
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	commandList_->SetPipelineState(pipelineState_->GetPipelineStateObject());
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// CBVをセット(ワールド行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kRootNode), nodeResource_->GetGPUVirtualAddress());
	//commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kViewProjection), camera.GetConstBuff()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kMaterial), materialResource_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kLight), directionalLightResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kCamera), camera_->GetCameraForGPU()->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kPointLight), pointLightResource_->GetGPUVirtualAddress());


	// SRVをセット
	if (textureHandle_ != TextureManager::Load(modelData_.material.textureFilePath)) {
		textureHandle_ = TextureManager::Load(modelData_.material.textureFilePath);
	}
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(RootBindings::kTexture), textureHandle_);

	commandList_->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

void Model::SetPointLight(const PointLight& pointLight)
{
	pointLightData_->color = pointLight.color;
	pointLightData_->position = pointLight.position;
	pointLightData_->intensity = pointLight.intensity;
}

//void Model::SetModelData(const std::string& fileName, const std::string format)
//{
//	modelData_ = *ModelManager::GetInstance()->Load(fileName, format);
//}

void Model::PlayingAnimation()
{
	animationTime_ += 1.0f / 60.0f;
	animationTime_ = std::fmod(animationTime_, animation_.duration);
	NodeAnimation& rootNodeAnimation = animation_.nodeAnimations[modelData_.rootNode.name];
	Vector3 translate = CalculateValue(rootNodeAnimation.translate, animationTime_);
	Quaternion rotate = CalculateQuaternion(rootNodeAnimation.rotate, animationTime_);
	//Vector3 scale = CalculateValue(rootNodeAnimation.scale, animationTime_);
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	Matrix4x4 localMatrix = MakeAffineMatrix(scale, rotate, translate);

	*nodeData_ = localMatrix;
}

//void T::SetMaterial(const Vector4& color)
//{
//	color;
//}

void Model::CreateMesh()
{	


	// 頂点リソース
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size()); // 使用するリソースのサイズは頂点サイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_->Map(0, nullptr, (void**)&vertexData_); // 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー


	/*HRESULT result = S_FALSE;
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
	}*/

}

void Model::InitializeDirectionalLight()
{
	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLight));
	directionalLightData_ = nullptr;
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// デフォルト値
	directionalLightData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLightData_->direction = Vector3(0.0f, -1.0f, 0.0f);
	directionalLightData_->intensity = 0.0f;

	pointLightResource_ = CreateBufferResource(sizeof(PointLight));
	pointLightData_ = nullptr;
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

	pointLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pointLightData_->position = { 0.0f, 2.0f, 0.0f };
	pointLightData_->intensity = 1.0f;


}

void Model::InitializeMaterial()
{
	materialResource_ = CreateBufferResource(sizeof(MaterialData));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = 0;
	materialData_->shininess = 50.0f;
}

void Model::InitializeNode()
{
	nodeResource_ = CreateBufferResource(sizeof(Matrix4x4));
	nodeData_ = nullptr;
	nodeResource_->Map(0, nullptr, reinterpret_cast<void**>(&nodeData_));
	*nodeData_ = modelData_.rootNode.localMatrix;
}

ID3D12Resource* Model::CreateBufferResource(size_t sizeInBytes)
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
	result = Device::GetInstance()->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));
	return resource;
}
