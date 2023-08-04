#pragma once

#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <wrl.h>

#include "Vector4.h"

class Triangle
{
public:

public:

	void InitializeRootSignature();

	void InitializeGraphicsPipeline();

	void CreateVertexResource();

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	void StaticInitialize(ID3D12Device* device);

	void Draw();

	
	
private:
	void Log(const std::string& message);

	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	std::vector<Vector4> vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
};

