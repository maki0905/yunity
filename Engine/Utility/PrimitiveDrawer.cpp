#include "PrimitiveDrawer.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "Device.h"
#include "ShaderCompiler.h"
#include "Common.h"
#include <GraphicsPipelineManager.h>

ID3D12GraphicsCommandList* yunity::PrimitiveDrawer::commandList_ = nullptr;
yunity::PipelineType yunity::PrimitiveDrawer::pipelineType_ = yunity::PipelineType::kCount;

void yunity::PrimitiveDrawer::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(commandList_ == nullptr);

	commandList_ = commandList;
}

void yunity::PrimitiveDrawer::PreDraw(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType)
{
	assert(commandList_ == nullptr);
	commandList_ = commandList;
	pipelineType_ = pipelineType;
}

void yunity::PrimitiveDrawer::PostDraw()
{
	commandList_ = nullptr;
	if (pipelineType_ != PipelineType::kCount) {
		pipelineType_ = PipelineType::kCount;
	}
}

yunity::PrimitiveDrawer* yunity::PrimitiveDrawer::Create(Type type)
{
	std::unique_ptr<PrimitiveDrawer> primitiveDrawer = std::make_unique<PrimitiveDrawer>();
	switch (type)
	{
	case PrimitiveDrawer::Type::kBox:
		primitiveDrawer->CreateBox();
		break;
	case PrimitiveDrawer::Type::kSphere:
		primitiveDrawer->CreateSphere();
		break;
	default:
		primitiveDrawer->CreateLine();
		break;
	}
	primitiveDrawer->CreateMesh();
	return primitiveDrawer.release();
}


void yunity::PrimitiveDrawer::Draw(const WorldTransform& worldTransform)
{
	if (commandList_ != nullptr) {
		//assert(commandList_);
		assert(worldTransform.constBuff_.Get());

		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, PipelineType::kPrimitive, BlendModeType::kNone);
		if (pipelineType_ != PipelineType::kCount) {
			GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, pipelineType_, BlendModeType::kNone);
		}
		commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		// 頂点バッファの設定
		commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

		commandList_->IASetIndexBuffer(&indexBufferView_);

		// CBVをセット(ワールド行列)
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());

		// CBVをセット(ビュープロジェクション行列)
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());

		commandList_->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);
	}
}

void yunity::PrimitiveDrawer::Draw(const Vector3& start, const Vector3& end, const Vector4& color)
{
	if (commandList_ != nullptr) {
		//assert(commandList_);

		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, PipelineType::kLine, BlendModeType::kNone);
		commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		vertexData_[0].position = { start.x, start.y, start.z, 1.0f };
		vertexData_[0].color = color;
		vertexData_[1].position = { end.x, end.y, end.z, 1.0f };
		vertexData_[1].color = color;

		// 頂点バッファの設定
		commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

		// CBVをセット(ビュープロジェクション行列)
		commandList_->SetGraphicsRootConstantBufferView(0, camera_->GetConstBuff()->GetGPUVirtualAddress());

		commandList_->DrawInstanced(2, 1, 0, 0);
	}
}

void yunity::PrimitiveDrawer::SetColor(const Vector4& color)
{
	for (auto& vertex : vertices_) {
		vertex.color = color;
	}
	std::memcpy(vertexData_, vertices_.data(), sizeof(VertexData) * vertices_.size());
}

void yunity::PrimitiveDrawer::CreateMesh()
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

	if (indices_.size() != 0) {
		indexBuff_ = CreateBufferResource(sizeof(uint32_t) * indices_.size()/** 16 * 16 * 6*/);
		indexBufferView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
		indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indices_.size()/** 16 * 16 * 6*/);
		indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

		//uint32_t* indexData = nullptr;
		indexBuff_->Map(0, nullptr, (void**)&indexData_/*indexData_*/);
		std::memcpy(indexData_, indices_.data(), sizeof(uint32_t) * indices_.size());
	}

}

void yunity::PrimitiveDrawer::CreateSphere()
{
	const uint32_t kSubdivision = 16;
	
	// 経度分割1つ分の角度 φ
	const float kLonEvery = float(std::numbers::pi) * 2.0f / float(kSubdivision);
	// 緯度分割1つ分の角度 θ
	const float kLatEvery = float(std::numbers::pi) / float(kSubdivision);
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(std::numbers::pi) / 2.0f + kLatEvery * latIndex;// θ
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			//uint32_t start = (latIndex * kSubdivision + lonIndex) * 4;
			float lon = lonIndex * kLonEvery;// φ

			VertexData vertexData[4];
			// 頂点にデータを入力する。基準点a
			vertexData[0].position.x = cos(lat) * cos(lon);
			vertexData[0].position.y = sin(lat);
			vertexData[0].position.z = cos(lat) * sin(lon);
			vertexData[0].position.w = 1.0f;
			vertexData[0].color = { 1, 0, 0, 1 };
			// b
			vertexData[1].position.x = cos(lat + kLatEvery) * cos(lon);
			vertexData[1].position.y = sin(lat + kLatEvery);
			vertexData[1].position.z = cos(lat + kLatEvery) * sin(lon);
			vertexData[1].position.w = 1.0f;
			vertexData[1].color = { 1, 0, 0, 1 };
			// c
			vertexData[2].position.x = cos(lat) * cos(lon + kLonEvery);
			vertexData[2].position.y = sin(lat);
			vertexData[2].position.z = cos(lat) * sin(lon + kLonEvery);
			vertexData[2].position.w = 1.0f;
			vertexData[2].color = { 1, 0, 0, 1 };
			// d
			vertexData[3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			vertexData[3].position.y = sin(lat + kLatEvery);
			vertexData[3].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			vertexData[3].position.w = 1.0f;
			vertexData[3].color = { 1, 0, 0, 1 };

			for (uint32_t i = 0; i < 4; i++) {
				vertices_.push_back(vertexData[i]);
			}

			
		}

	}

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			//uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			uint32_t i = (latIndex * kSubdivision + lonIndex) * 4;

			uint32_t indexData[6];
			indexData[0] = i;
			indexData[1] = i + 1;
			indexData[2] = i + 2;
			indexData[3] = i + 1;
			indexData[4] = i + 3;
			indexData[5] = i + 2;

			for (uint32_t index = 0; index < 6; index++) {
				indices_.push_back(indexData[index]);
			}

		}

	}

	type_ = Type::kSphere;
}

void yunity::PrimitiveDrawer::CreateBox()
{


	VertexData vertexData[8];
	vertexData[0] = { Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertexData[1] = { Vector4(-0.5f, 0.5f, -0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertexData[2] = { Vector4(0.5f, 0.5f, -0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertexData[3] = { Vector4(0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertexData[4] = { Vector4(-0.5f, -0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertexData[5] = { Vector4(-0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertexData[6] = { Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertexData[7] = { Vector4(0.5f, -0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };

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

	type_ = Type::kBox;
}

void yunity::PrimitiveDrawer::CreateLine()
{
	VertexData vertexData[2];
	vertexData[0] = { Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertexData[1] = { Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) };

	for (int32_t i = 0; i < 2; i++) {
		vertices_.push_back(vertexData[i]);
	}

	type_ = Type::kLine;
}


