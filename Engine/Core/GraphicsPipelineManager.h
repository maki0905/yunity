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
	kLine,
	kPrimitive,
	kSkinning,
	kCount,
};

class GraphicsPipelineManager {
	/*struct GraphicsPipeline {
		std::unordered_map<BlendModeType, PipelineState*> pso_;
	};*/
	struct GraphicsPipeline {
		std::unique_ptr<PipelineState> pso_[BlendModeType::kBlendCount];
		std::unique_ptr<RootSignature> rooSignature_;
	};
public:
	static GraphicsPipelineManager* GetInstance();
	void Initialize();
	void SetCommandList(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendModeType blendModeType);

private:
	void CreateObject3d();
	void CreateSprite();
	void CreateParticle();
	void CreateLine();
	void CreatePrimitive();
	void CreateSkinning();

private:
	GraphicsPipelineManager() = default;
	~GraphicsPipelineManager() = default;
	GraphicsPipelineManager(const GraphicsPipelineManager&) = delete;
	GraphicsPipelineManager& operator=(const GraphicsPipelineManager&) = delete;
private:
	//Microsoft::WRL::ComPtr<ID3D12Device> device_;
	GraphicsCommon* graphicsCommon_;
	std::unique_ptr<GraphicsPipeline> graphicsPipelines_[PipelineType::kCount];
	//GraphicsPipeline* graphicsPipelines_[PipelineType::kCount];

};