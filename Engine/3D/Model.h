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
#include "Animation.h"

class PipelineState;
class RootSignature;

class Model
{
	/// <summary>
		/// ルートパラメータ番号
		/// </summary>
	enum class RootBindings {
		kWorldTransform, // ワールド変換行列
		kViewProjection, // ビュープロジェクション変換行列
		kRootNode,       // ルートノード
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

	struct MaterialData {
		Vector4 color;
		uint32_t enableLighting;
		float shininess;
		std::string textureFilePath;
	};

	struct Node {
		Matrix4x4 localMatrix;
		std::string name;
		std::vector<Node> children;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
		Node rootNode;
	};

	struct DirectionalLight {
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity = 0; // 輝度
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
	static Model* Create(const std::string& fileName, const std::string format);

	/// <summary>
	/// グラフィックスパイプライン生成
	/// </summary>
	static void InitializeGraphicsPipeline();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const ModelData& modelData, const Animation& animation);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Draw(const WorldTransform& worldTransform, uint32_t textureHandle);
	void Draw(const WorldTransform& worldTransform/*, const Camera& camera*/);

	//void SetMaterial(const Vector4& color);

	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetPointLight(const PointLight& pointLight);
	void SetLighting(bool flag) { materialData_->enableLighting = flag; }

	//void SetModelData(const std::string& fileName, const std::string format);

	bool IsAnimation() { return isAnimation_; }
	void PlayAnimation() { isAnimation_ = true; }
	void StopAnimation() { isAnimation_ = false; }
	void PlayingAnimation();

private:

	// メッシュ生成
	void CreateMesh();

	void InitializeDirectionalLight();

	void InitializeMaterial();

	void InitializeNode();

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

	ModelData modelData_;
	Animation animation_;
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

	Microsoft::WRL::ComPtr<ID3D12Resource> nodeResource_;
	Matrix4x4* nodeData_;
	float animationTime_ = 0.0f;
	bool isAnimation_ = false;

};

