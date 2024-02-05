#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <memory>
#include <wrl.h>

#include "TextureManager.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "MathFunction.h"

class PipelineState;
class RootSignature;

class PrimitiveDrawer
{

	/// <summary>
		/// ルートパラメータ番号
		/// </summary>
	enum class RootBindings {
		kWorldTransform, // ワールド変換行列
		kViewProjection, // ビュープロジェクション変換行列
		kCount,          // 最大数
	};

public:
	// 図形
	enum class Type {
		kBox,
		kSphere,
	};

	// 頂点データ構造体
	struct VertexData {
		Vector4 position;
		Vector4 color;
	};

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dモデル生成
	/// </summary>
	/// <returns></returns>
	static PrimitiveDrawer* Create(Type type);

	/// <summary>
	/// グラフィックスパイプライン生成
	/// </summary>
	static void InitializeGraphicsPipeline();



public:

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const WorldTransform& worldTransform);

	void SetCamera(Camera* camera) { camera_ = camera; }

private:

	// メッシュ生成
	void CreateMesh();

	void CreateSphere();

	void CreateBox();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns>サイズ</returns>
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

private:
	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static RootSignature* rootSignature_;
	static PipelineState* pipelineState_;

private:
	Camera* camera_ = nullptr;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	// 頂点データ
	VertexData* vertexData_;
	std::vector<VertexData> vertices_;

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	// 頂点インデックスデータ
	std::vector<uint32_t> indices_;
	uint32_t* indexData_;

private:




};

