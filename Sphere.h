﻿#pragma once

#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <wrl.h>
#include "MathFunction.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Vecter2.h"
#include "Matrix4x4.h"

class Sphere
{
public:
	enum class RoomParameter {
		kMaterial,       // マテリアル
		kWorldTransform, // ワールド変換行列
		kViewProjection, // ビュープロジェクション変換行列
		kTexture,        // テクスチャ
	};

public:
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct Material {
		Vector4 color;
		int32_t enableLighting;
	};
	struct DirectionalLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

public:

	static void InitializeRootSignature();

	static void InitializeGraphicsPipeline();

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	static void StaticInitialize(ID3D12Device* device);

	static Sphere* Create();

	void Initialize();

	void Update();

	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHandle = 0);

	void CreateMesh();


private:
	static void Log(const std::string& message);

	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	uint32_t* indexData_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Vector4* materialData_;


};

