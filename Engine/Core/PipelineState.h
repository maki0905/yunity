#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "RootSignature.h"

namespace yunity {
    /*
    * @brief パイプラインステートクラス
    */
    class PipelineState
    {
    public:
        enum class ShaderType {
            kVS,
            kPS,
        };
    public:
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="device"></param>
        /// <param name="rootSignature"></param>
        PipelineState(ID3D12Device* device, RootSignature* rootSignature);

        /// <summary>
        /// BlendDesc設定
        /// </summary>
        /// <param name="blendDesc"></param>
        void SetBlendState(const D3D12_BLEND_DESC& blendDesc);

        /// <summary>
        /// BlendDesc設定
        /// </summary>
        /// <param name="blendDesc"></param>
        void SetBlendState(const D3D12_RENDER_TARGET_BLEND_DESC& blendDesc);

        /// <summary>
        /// RasterizerState設定
        /// </summary>
        /// <param name="rasterizerDesc"></param>
        void SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc);

        /// <summary>
        /// DepthStencilState設定
        /// </summary>
        /// <param name="depthStencilDesc"></param>
        void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);

        /// <summary>
        /// SampleMask設定
        /// </summary>
        /// <param name="sampleMask"></param>
        void SetSampleMask(UINT sampleMask);

        /// <summary>
        /// PrimitiveTopologyType設定
        /// </summary>
        /// <param name="topologyType"></param>
        void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType);

        /// <summary>
        /// DepthTargetFormat設定
        /// </summary>
        /// <param name="dsvFormat"></param>
        /// <param name="msaaCount"></param>
        /// <param name="msaaQuality"></param>
        void SetDepthTargetFormat(DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);

        /// <summary>
        /// RenderTargetFormat設定
        /// </summary>
        /// <param name="rtvFormat"></param>
        /// <param name="dsvFormat"></param>
        /// <param name="msaaCount"></param>
        /// <param name="msaaQuality"></param>
        void SetRenderTargetFormat(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);

        /// <summary>
        /// RenderTargetFormats設定
        /// </summary>
        /// <param name="numRTVs"></param>
        /// <param name="rtvFormats"></param>
        /// <param name="dsvFormat"></param>
        /// <param name="msaaCount"></param>
        /// <param name="msaaQuality"></param>
        void SetRenderTargetFormats(UINT numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);

        /// <summary>
        /// InputLayout設定
        /// </summary>
        /// <param name="inputLayoutDesc"></param>
        void SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);

        /// <summary>
        /// PrimitiveRestart設定
        /// </summary>
        /// <param name="IBProps"></param>
        void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps);

        /// <summary>
        /// Shader設定
        /// </summary>
        /// <param name="type"></param>
        /// <param name="blob"></param>
        void SetShader(ShaderType type, Microsoft::WRL::ComPtr<ID3DBlob> blob);

        /// <summary>
        /// DepthFunc設定
        /// </summary>
        /// <param name="depthFunc"></param>
        void SetDepthFunc(D3D12_COMPARISON_FUNC depthFunc);

        /// <summary>
        /// CullMode設定
        /// </summary>
        /// <param name="cullMode"></param>
        void SetCullMode(D3D12_CULL_MODE cullMode);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="format"></param>
        void SetDSVFormat(DXGI_FORMAT format);

        /// <summary>
        /// ファイナライザ
        /// </summary>
        void Finalize();

        ID3D12PipelineState* GetPipelineStateObject() { return graphicsPipelineState_.Get(); }

    protected:
        ID3D12Device* device_ = nullptr;
        RootSignature* rootSignature_ = nullptr;

        Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc_{};

    };
}
