#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <memory>
#include <wrl.h>
#include <optional>
#include <span>

#include "TextureManager.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "MathFunction.h"
#include "PointLight.h"
#include "Animation.h"
#include "GraphicsPipelineManager.h"
#include "DirectionLight.h"


class PipelineState;
class RootSignature;

namespace yunity {
	enum ModelType {
		kRigid,    // リジッドメッシュ
		kKeyframe, // リジッドメッシュ(キーフレームアニメーション)
		kSkin, // スキンメッシュ(スキンアニメーション)
	};

	/*
	* @brief 3Dモデルクラス
	*/
	class Model
	{
	public:
		// 頂点データ構造体
		struct VertexData {
			Vector4 position;
			Vector2 texcoord;
			Vector3 normal;
		};

		// マテリアル構造体
		struct MaterialData {
			Vector4 color;
			uint32_t enableLighting;
			float shininess;
			std::string textureFilePath;
		};

		// 
		struct Node {
			QuaternionTransform transform;
			Matrix4x4 localMatrix;
			std::string name;
			std::vector<Node> children;
		};

		// 
		struct VertexWeightData {
			float weight;
			uint32_t vertexIndex;
		};

		// 
		struct JointWeightData {
			Matrix4x4 inverseBindPoseMatrix;
			std::vector<VertexWeightData> vertexWeights;
		};

		// 
		struct ModelData {
			std::map<std::string, JointWeightData> skinClusterData;
			std::vector<VertexData> vertices;
			std::vector<uint32_t> indices;
			MaterialData material;
			Node rootNode;
		};

		// 
		static const uint32_t kNumMaxInfluence = 4;

		// 
		struct VertexInfluence {
			std::array<float, kNumMaxInfluence> weights;
			std::array<int32_t, kNumMaxInfluence> jointIndices;
		};

		// 
		struct WellForGPU {
			Matrix4x4 skeletonSpaceMatrix; // 位置用
			Matrix4x4 skeletonSpaceInverseTransposeMatrix; // 法線用
		};

		// 
		struct SkinCluster {
			std::vector<Matrix4x4> inverseBindPoseMatrices;
			Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
			D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
			std::span<VertexInfluence> mappedInfluence;
			Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
			std::span<WellForGPU> mappedPalette;
			std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
		};

		// 
		struct DirectionalLight {
			Vector4 color; // ライトの色
			Vector3 direction; // ライトの向き
			float intensity = 0; // 輝度
		};

		// 
		struct Joint {
			QuaternionTransform transform; // Transform情報
			Matrix4x4 localMatrix; // localMatrix
			Matrix4x4 skeletonSpaceMatrix; // skeletonSpaceでの変換行列
			std::string name; // 名前
			std::vector<int32_t> children; // 子JointのIndexリスト。いなければ空
			int32_t index; // 自身のIndex
			std::optional<int32_t> parent; // 親JointのIndex。いなければnull
		};

		// 
		struct Skeleton {
			int32_t root; // RootJointのIndex
			std::map<std::string, int32_t> jointMap; // Joint名とIndexとの辞書
			std::vector<Joint> joints; // 所属しているジョイント
		};

	public:

		/// <summary>
		/// 描画前処理
		/// </summary>
		/// <param name="commandList">描画コマンドリスト</param>
		static void PreDraw(ID3D12GraphicsCommandList* commandList);

		/// <summary>
		/// 描画前処理
		/// </summary>
		/// <param name="commandList"></param>
		/// <param name="pipelineType"></param>
		static void PreDraw(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType);

		/// <summary>
		/// 描画後処理
		/// </summary>
		static void PostDraw();

	public:
		/// <summary>
		/// デストラクタ
		/// </summary>
		~Model();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(const std::string& name, const ModelType& modelType, const ModelData& modelData);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="worldTransform">ワールドトランスフォーム</param>
		/// <param name="viewProjection">ビュープロジェクション</param>
		/// <param name="textureHandle">テクスチャハンドル</param>
		void Draw(const WorldTransform& worldTransform, uint32_t textureHandle);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="worldTransform">ワールドトランスフォーム</param>
		void Draw(const WorldTransform& worldTransform);

		//void SetMaterial(const Vector4& color);

		/// <summary>
		/// setter
		/// </summary>

		void SetCamera(Camera* camera) { camera_ = camera; }
		void SetPointLight(const PointLight& pointLight);
		void SetEnableLighting(bool flag) { materialData_->enableLighting = flag; }
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetBlendModeType(const BlendModeType& blendModeType) { blendModeType_ = blendModeType; }
		void SetAnimation(std::string name, const Animation& animation, AnimationCommon::AnimationMode mode = AnimationCommon::AnimationMode::kStopped);
		bool IsAnimation() { return isAnimation_; }
		void PlayAnimation() { isAnimation_ = true; }
		void SetDirectionLight(DirectionLight* directionLight) { directionLight_ = directionLight; }

		/// <summary>
		/// getter
		/// </summary>
		/// 
		Camera* GetCamera() { return camera_; }
		std::string GetModelName() { return modelName_; }
		ModelData GetModelData() { return modelData_; }

		//void SetModelData(const std::string& fileName, const std::string format);

		/// <summary>
		/// アニメーション再生
		/// </summary>
		/// <param name="name">アニメーション名</param>
		/// <param name="mode">アニメーションモード</param>
		void PlayAnimation(std::string name, AnimationCommon::AnimationMode mode = AnimationCommon::AnimationMode::kPlaying);

		/// <summary>
		/// 全アニメーション停止
		/// </summary>
		void StopAnimation();

		/// <summary>
		/// 指定アニメーション停止
		/// </summary>
		/// <param name="name">アニメーション名</param>
		void StopAnimation(std::string name);

		/// <summary>
		/// 位置アニメーション
		/// </summary>
		/// <param name="from">始点</param>
		/// <param name="to">終点</param>
		/// <param name="time">タイム</param>
		void TransitionAnimation(const std::string& from, const std::string& to, float time);

		/// <summary>
		/// スケルトン更新
		/// </summary>
		void SkeletonUpdate();

		/// <summary>
		/// アニメーション更新
		/// </summary>
		void ApplyAnimation();

		/// <summary>
		/// スキンクラスタ更新
		/// </summary>
		void SkinClusterUpdate();

	private:

		/// <summary>
		/// メッシュ生成
		/// </summary>
		void CreateMesh();

		/// <summary>
		/// インデックス生成
		/// </summary>
		void CreateIndex();

		/// <summary>
		/// ライト初期化
		/// </summary>
		void InitializeDirectionalLight();

		/// <summary>
		/// マテリアル初期化
		/// </summary>
		void InitializeMaterial();

		/// <summary>
		/// ノード初期化
		/// </summary>
		void InitializeNode();

		/// <summary>
		/// 定数バッファ生成
		/// </summary>
		/// <param name="sizeInBytes">サイズ</param>
		/// <returns>サイズ</returns>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rootNode">ルートノード</param>
		/// <returns>スケルトン</returns>
		Skeleton CreateSkelton(const Node& rootNode);

		/// <summary>
		/// ジョイント生成
		/// </summary>
		/// <param name="node">ノード</param>
		/// <param name="parent">ペアレント</param>
		/// <param name="joints">ジョイントズ</param>
		/// <returns></returns>
		int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

		/// <summary>
		/// スキンクラスタ生成
		/// </summary>
		/// <returns></returns>
		SkinCluster CreateSkinCluster();

	private:
		static ID3D12GraphicsCommandList* commandList_;
		static PipelineType pipelineType_;
	private:
		ModelType modelType_ = ModelType::kRigid;
		BlendModeType blendModeType_ = BlendModeType::kNormal;
		Camera* camera_ = nullptr;

		ModelData modelData_;
		Skeleton skeleton_;
		SkinCluster skinCluster_;
		std::vector<std::string> animationNames_;
		std::unordered_map<std::string, AnimationData> animations_;
		// 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
		// 頂点データ
		VertexData* vertexData_;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

		// インデックス
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
		D3D12_INDEX_BUFFER_VIEW indexBufferView_;
		uint32_t* mappedIndex_;

		// ライティング
		Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;
		DirectionalLight* directionalLightData_;

		Microsoft::WRL::ComPtr<ID3D12Resource> lightVPBuff_;
		ConstBufferDataViewProjection* lightVP = nullptr;
		DirectionLight* directionLight_;

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
		bool isTransition_ = false;
		std::string presentAnimation_;
		std::string from_;
		std::string to_;
		float transitionTime_;
		float transitionSecound_;

		std::string modelName_;
	};
}


