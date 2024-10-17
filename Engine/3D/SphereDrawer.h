#pragma once

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
#include "PointLight.h"


class PipelineState;
class RootSignature;

/*
* @brief 球クラス
*/
class SphereDrawer
{
	/// <summary>
		/// ルートパラメータ番号
		/// </summary>
	enum class RootBindings {
		kWorldTransform, // ワールド変換行列
		kViewProjection, // ビュープロジェクション変換行列
		kTexture,        // テクスチャ
		kMaterial,       // マテリアル
		kLight,          // ライティング
		kCamera,         // カメラ
		kPointLight,
		kCount,          // 最大数
	};

public:
	// 頂点データ構造体
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	// 
	struct MaterialData {
		Vector4 color;
		uint32_t enableLighting;
		float shininess;
		std::string textureFilePath;
	};

	// 
	struct DirectionalLight {
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity; // 輝度
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
	static SphereDrawer* Create(const std::string& textureName);

	/// <summary>
	/// グラフィックスパイプライン生成
	/// </summary>
	static void InitializeGraphicsPipeline();



public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	void Draw(const WorldTransform& worldTransform);

	/// <summary>
	/// setter
	/// </summary>

	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetTextureHandle(const std::string& textureName);
	void SetPointLight(const PointLight& pointLight);


private:
	/// <summary>
	/// メッシュ生成
	/// </summary>
	void CreateMesh();

	/// <summary>
	/// ライト初期化
	/// </summary>
	void InitializeDirectionalLight();

	/// <summary>
	/// マテリアル初期化
	/// </summary>
	void InitializeMaterial();

private:
	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static RootSignature* rootSignature_;
	static PipelineState* pipelineState_;

private:

	Camera* camera_ = nullptr;

	const uint32_t kSubdivision = 16;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	// 頂点データ
	VertexData* vertexData_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	// インデックスデータ
	uint32_t* indexData_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	// ライティング
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;
	DirectionalLight* directionalLightData_;


	// マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	MaterialData* materialData_;

	// テクスチャハンドル
	uint32_t textureHandle_;

	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_;
	PointLight* pointLightData_;
};

