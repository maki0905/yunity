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

enum PipelineType {
	kObject3d,
	kSprite,
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
	void CreateObject3d();
	void CreateSprite();
	void CreateParticle();
	void CreatePrimitive();
	void CreateSkinning();

private:
	GraphicsPipelineManager() = default;
	~GraphicsPipelineManager() = default;
	GraphicsPipelineManager(const GraphicsPipelineManager&) = delete;
	GraphicsPipelineManager& operator=(const GraphicsPipelineManager&) = delete;
private:
	ID3D12Device* device_;
	GraphicsCommon* graphicsCommon_;
	GraphicsPipeline* graphicsPipelines_[PipelineType::kCount];

};