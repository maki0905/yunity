#pragma once

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
#include "Matrix4x4.h"

class Triangle
{
public:
	enum class RoomParameter {
		kWorldTransform, // ワールド変換行列
		kViewProjection, // ビュープロジェクション変換行列
		kTexture,        // テクスチャ
	};

public:

	static void InitializeRootSignature();

	static void InitializeGraphicsPipeline();

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	static void StaticInitialize(ID3D12Device* device);

	static Triangle* Create();

	void Initialize();

	void Update();

	void Draw(const WorldTransform& worldTransform);

	void CreateMesh();
	
	
private:
	static void Log(const std::string& message);

	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	std::vector<Vector4> vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Vector4* materialData_;
	
};

