#pragma once

#include <d3d12.h>
#include <unordered_map>
#include <wrl.h>
#include <memory>

#include "PipelineState.h"
#include "RootSignature.h"
#include "GraphicsCommon.h"

//class PipelineState;
//class RootSignature;
//class GraphicsCommon;

enum BlendModeType {
	kBlendModeNone, //!< ブレンドなし
	kBlendModeNormal, //!< 通常αブレンド
	kBlendModeAdd, //!< 加算
	kBlendModeSubtract, //!< 減算
	kBlendModeMultily, //!< 乗算
	kBlendModeScreen, //!< スクリーン
	kBlendCount, // 最大数
};

enum PipelineType {
	kObject3d,
	kTexture,
	kParticle,
	kPrimitive,
	kSkinning,
	kCount,
};

class GraphicsPipelineManager {
	/*struct GraphicsPipeline {
		std::unordered_map<BlendModeType, PipelineState*> pso_;
	};*/
	struct GraphicsPipeline {
		PipelineState* pso_[BlendModeType::kBlendCount];
		RootSignature* rooSignature_;
	};
public:
	static GraphicsPipelineManager* GetInstance();
	void Initialize();
	void SetCommandList(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendModeType blendModeType);
private:
	GraphicsPipelineManager() = default;
	~GraphicsPipelineManager() = default;
	GraphicsPipelineManager(const GraphicsPipelineManager&) = delete;
	GraphicsPipelineManager& operator=(const GraphicsPipelineManager&) = delete;
private:
	std::unique_ptr<GraphicsCommon> graphicsCommon_;
	//std::unordered_map<PipelineType, GraphicsPipeline*> graphicsPipelines_;
	GraphicsPipeline* graphicsPipelines_[PipelineType::kCount];
	//std::unordered_map<PipelineType, RootSignature*> rootSignatures_;
};