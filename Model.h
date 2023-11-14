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

/// <summary>
/// 3Dモデル
/// </summary>
class Model {
	/// <summary>
	/// ルートパラメータ番号
	/// </summary>
	enum class RoomParameter {
		kWorldTransform, // ワールド変換行列
		kViewProjection, // ビュープロジェクション変換行列
		kTexture,        // テクスチャ
	};

public:
	// 頂点データ構造体
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct MaterialData {
		std::string textureFilePath;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

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
	static Model* Create(const std::string& modelname);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHandle);
	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

private:
	/// <summary>
	/// ログ
	/// </summary>
	/// <param name="message"></param>
	static void Log(const std::string& message);

	/// <summary>
	/// ルートシグネチャー生成
	/// </summary>
	static void InitializeRootSignature();

	/// <summary>
	/// グラフィックスパイプライン生成
	/// </summary>
	static void InitializeGraphicsPipeline();

	// メッシュ生成
	void CreateMesh();

	// オブジェファイル読み込み
	void LoadObjFile(const std::string& filename);

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns>サイズ</returns>
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);
	// デバイス
	static ID3D12Device* device_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* commandList_;
	// ルートシグネチャー
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

private:
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

};