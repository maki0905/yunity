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
#include "DescriptorHeap.h"
#include "Particle.h"
#include "Model.h"

namespace yunity {
	class ParticleDrawer
	{
		/// <summary>
			/// ルートパラメータ番号
			/// </summary>
		enum class RootBindings {
			kWorldTransform, // ワールド変換行列
			kViewProjection, // ビュープロジェクション変換行列
			kTexture,        // テクスチャ
			kMaterial,       // マテリアル
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
			std::string textureFilePath;
		};

		struct ModelData {
			std::vector<VertexData> vertices;
			std::vector<uint32_t> indices;
			MaterialData material;
		};

	public:

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
		static ParticleDrawer* Create(const std::string& filename);

	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(const std::string& filename = nullptr);

		void Initialize(const Model::ModelData& modelData);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="worldTransform">ワールドトランスフォーム</param>
		/// <param name="viewProjection">ビュープロジェクション</param>
		/// <param name="textureHandle">テクスチャハンドル</param>
		void Draw(std::list<Particle> particles);

		Vector4 GetMatrial() { return materialData_->color; }

		void SetCamera(yunity::Camera* camera) { camera_ = camera; }
		void SetMaterial(const Vector4& color);

		void SetTextuer(const std::string& filename) { textureHandle_ = yunity::TextureManager::Load(filename); }
		void SetBlendModeType(const BlendModeType& blendModeType) { blendModeType_ = blendModeType; }

	private:

		// メッシュ生成
		void CreateMesh();
		void CreateMesh(const std::vector<Model::VertexData>& vertices);

		// インデックス生成
		void CreateIndex(const std::vector<uint32_t>& indices);

		void CreateSRV();

		void InitializeMaterial();

	private:

		static ID3D12GraphicsCommandList* commandList_;

	private:
		// ブレンドモード
		BlendModeType blendModeType_ = BlendModeType::kNone;

		yunity::Camera* camera_ = nullptr;

		// SRV用ヒープ
		yunity::DescriptorHeap* srvHeap_;

		ModelData modelData_;
		// 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
		// 頂点データ
		VertexData* vertexData_;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
		// インデックスバッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView_;
		uint32_t* mappedIndex_;

		// インスタンス数
		const uint32_t kNumMaxInstance = 100;

		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;
		ParticleForCPU* instancingData_;

		D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;
		D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;

		// マテリアル
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
		MaterialData* materialData_;

		// テクスチャハンドル
		uint32_t textureHandle_;
	};
}

