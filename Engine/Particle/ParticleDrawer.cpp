#include "ParticleDrawer.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "Device.h"
#include "ShaderCompiler.h"
#include "DirectXCore.h"
#include "Common.h"

ID3D12GraphicsCommandList* yunity::ParticleDrawer::commandList_ = nullptr;
yunity::PipelineType yunity::ParticleDrawer::pipelineType_ = yunity::PipelineType::kCount;

void yunity::ParticleDrawer::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(commandList_ == nullptr);

	commandList_ = commandList;
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void yunity::ParticleDrawer::PreDraw(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType)
{
	assert(commandList_ == nullptr);
	commandList_ = commandList;
	pipelineType_ = pipelineType;
}

void yunity::ParticleDrawer::PostDraw()
{
	commandList_ = nullptr;
	if (pipelineType_ != PipelineType::kCount) {
		pipelineType_ = PipelineType::kCount;
	}
}

yunity::ParticleDrawer* yunity::ParticleDrawer::Create(const std::string& modelname)
{
	std::unique_ptr<ParticleDrawer> particleDrawer = std::make_unique<ParticleDrawer>();
	particleDrawer->Initialize(modelname);
	return particleDrawer.release();
}


void yunity::ParticleDrawer::Initialize(const std::string& filename)
{
	CreateMesh();
	modelData_.indices.clear();
	if (filename.size() != 0) {
		textureHandle_ = TextureManager::Load(filename);
	}
	CreateSRV();
	InitializeMaterial();
}

void yunity::ParticleDrawer::Initialize(const Model::ModelData& modelData)
{
	CreateMesh(modelData.vertices);
	CreateIndex(modelData.indices);
	textureHandle_ = TextureManager::Load(modelData.material.textureFilePath);
	CreateSRV();
	InitializeMaterial();
}

void yunity::ParticleDrawer::Draw(std::list<Particle> particles)
{
	//assert(commandList_);

	if (commandList_ != nullptr) {
		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, PipelineType::kParticle, blendModeType_);
		if (pipelineType_ != PipelineType::kCount) {
			GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, pipelineType_, blendModeType_);
		}
		commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		uint32_t numInstance = 0;

		for (uint32_t index = 0; auto & particle : particles) {
			instancingData_[index].world = particle.particleForCPU.world;
			instancingData_[index].color = particle.particleForCPU.color;
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

		if (modelData_.indices.size() != 0) {
			commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), numInstance, 0, 0, 0);
		}
		else {
			commandList_->DrawInstanced(UINT(modelData_.vertices.size()), numInstance, 0, 0);
		}
	}

	
}

void yunity::ParticleDrawer::SetMaterial(const Vector4& color)
{
	materialData_->color = color;
}

void yunity::ParticleDrawer::CreateMesh()
{

	modelData_.vertices.push_back({ .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左上
	modelData_.vertices.push_back({ .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右上
	modelData_.vertices.push_back({ .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左下
	modelData_.vertices.push_back({ .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左下
	modelData_.vertices.push_back({ .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右上
	modelData_.vertices.push_back({ .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右下

	// 頂点リソース
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size()); // 使用するリソースのサイズは頂点サイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_->Map(0, nullptr, (void**)&vertexData_); // 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー
	vertexResource_->Unmap(0, nullptr);
}

void yunity::ParticleDrawer::CreateMesh(const std::vector<Model::VertexData>& vertices)
{
	modelData_.vertices.clear();
	for (auto& vertex : vertices) {
		modelData_.vertices.push_back({ .position = vertex.position, .texcoord = vertex.texcoord, .normal = vertex.normal }); // 左上
	}

	// 頂点リソース
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size()); // 使用するリソースのサイズは頂点サイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_->Map(0, nullptr, (void**)&vertexData_); // 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー
	vertexResource_->Unmap(0, nullptr);
}



void yunity::ParticleDrawer::CreateIndex(const std::vector<uint32_t>& indices)
{
	modelData_.indices.clear();
	for (auto& index : indices) {
		modelData_.indices.push_back(index);
	}

	// インデックスリソース
	indexResource_ = CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());
	// インデックスバッファビュー
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	// インデックスリソースにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex_));
	std::memcpy(mappedIndex_, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
	indexResource_->Unmap(0, nullptr);
}

void yunity::ParticleDrawer::CreateSRV()
{
	instancingResource_ = CreateBufferResource(sizeof(ParticleForCPU) * kNumMaxInstance);
	instancingResource_->Map(0, nullptr, (void**)&instancingData_);

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].world = MakeIdentity4x4();
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
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForCPU);
	instancingSrvHandleCPU_ = srvHandle.GetCPUHandle();
	instancingSrvHandleGPU_ = srvHandle.GetGPUHandle();
	Device::GetInstance()->GetDevice()->CreateShaderResourceView(instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

}


void yunity::ParticleDrawer::InitializeMaterial()
{
	materialResource_ = CreateBufferResource(sizeof(MaterialData));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialResource_->Unmap(0, nullptr);
}


