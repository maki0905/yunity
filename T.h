#pragma once

#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <memory>
#include <wrl.h>
#include "MathFunction.h"


class PipelineState;
class RootSignature;

class T
{
	/// <summary>
		/// ルートパラメータ番号
		/// </summary>
	enum class RoomParameter {
		kWorldTransform, // ワールド変換行列
		kViewProjection, // ビュープロジェクション変換行列
		kTexture,        // テクスチャ
		kMaterial,       // マテリアル
		kLight,          // ライティング
	};

public:
	// 頂点データ構造体
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct MaterialData {
		Vector4 color;
		std::string textureFilePath;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	struct DirectionalLight {
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity; // 輝度
	};

public:
	

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device);

	void Create();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHandle);
	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	void SetMaterial(const Vector4& color);

private:

	// メッシュ生成
	void CreateMesh();

	// オブジェファイル読み込み
	void LoadObjFile(const std::string& filename);

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	void InitializeDirectionalLight();

	void InitializeMaterial();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns>サイズ</returns>
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

private:
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;

	RootSignature* rootSignature_;
	PipelineState* PSO_;



	ModelData modelData;
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	// 頂点データ
	VertexData* vertexData_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	// インデックスデータ
	std::vector<uint16_t> indexData_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;
	// テクスチャハンドル
	uint32_t textureHandle = 0;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

	// ライティング
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;
	DirectionalLight* directionalLightData_;


	// マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	MaterialData* materialData_;

	// テクスチャハンドル
	uint32_t textureHandle_;
};

