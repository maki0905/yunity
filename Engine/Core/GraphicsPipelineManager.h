#pragma once

#include <d3d12.h>
#include <unordered_map>
#include <wrl.h>
#include <memory>

#include "PipelineState.h"
#include "RootSignature.h"
#include "GraphicsCommon.h"

namespace yunity {
	enum PipelineType {
		kObject3d,
		kSprite,
		kParticle,
		kLine,
		kPrimitive,
		kSkinning,
		kShadowMap,
		kObject3dShadowMap,
		kCount,
	};

	/*
	* @brief グラフィックパイプラインマネージャークラス
	*/
	class GraphicsPipelineManager {
		/*struct GraphicsPipeline {
			std::unordered_map<BlendModeType, PipelineState*> pso_;
		};*/

		// 
		struct GraphicsPipeline {
			std::unique_ptr<PipelineState> pso_[BlendModeType::kBlendCount];
			std::unique_ptr<RootSignature> rooSignature_;
		};
	public:
		/// <summary>
		/// インスタンス取得
		/// </summary>
		/// <returns></returns>
		static GraphicsPipelineManager* GetInstance();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// コマンドリスト設定
		/// </summary>
		/// <param name="commandList"></param>
		/// <param name="pipelineType"></param>
		/// <param name="blendModeType"></param>
		void SetCommandList(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendModeType blendModeType);

	private:

		/// <summary>
		/// 3Dオブジェクト用パイプライン生成
		/// </summary>
		void CreateObject3d();

		/// <summary>
		/// パーティクル用パイプライン生成
		/// </summary>
		void CreateParticle();

		/// <summary>
		/// ライン描画用パイプライン生成
		/// </summary>
		void CreateLine();

		/// <summary>
		/// プリミティブ用パイプライン生成
		/// </summary>
		void CreatePrimitive();

		/// <summary>
		/// スキニング用パイプライン生成
		/// </summary>
		void CreateSkinning();

		/// <summary>
		/// シャドウマップ用パイプライン生成
		/// </summary>
		void CreateShadowMap();

		/// <summary>
		/// シャドウマップ3Dオブジェクト用パイプライン生成
		/// </summary>
		void CreateObject3dShadowMap();

	private:
		GraphicsPipelineManager() = default;
		~GraphicsPipelineManager() = default;
		GraphicsPipelineManager(const GraphicsPipelineManager&) = delete;
		GraphicsPipelineManager& operator=(const GraphicsPipelineManager&) = delete;
	private:
		GraphicsCommon* graphicsCommon_;
		std::unique_ptr<GraphicsPipeline> graphicsPipelines_[PipelineType::kCount];

	};
}

