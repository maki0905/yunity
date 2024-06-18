#pragma once

#include <d3d12.h>

enum BlendModeType {
    kNone, //!< ブレンドなし
    kNormal, //!< 通常αブレンド
    kAdd, //!< 加算
    kSubtract, //!< 減算
    kMultily, //!< 乗算
    kScreen, //!< スクリーン
    kBlendCount, // 最大数
};

class GraphicsCommon {
public:
    static GraphicsCommon* GetInstance();
    void Initialize();

    // StaticSampler
    D3D12_STATIC_SAMPLER_DESC StaticSampler;

    // Rasterizer
    D3D12_RASTERIZER_DESC RasterizerDefault;        // 通常
    D3D12_RASTERIZER_DESC RasterizerDefaultMsaa;    // 通常+マルチサンプルアンチエイリアシング
    D3D12_RASTERIZER_DESC RasterizerDefaultCw;      // 時計回りのポリゴンをカリング
    D3D12_RASTERIZER_DESC RasterizerDefaultCwMsaa;  // 時計回りのポリゴンをカリング+マルチサンプルアンチエイリアシング
    D3D12_RASTERIZER_DESC RasterizerTwoSided;       // カリング無効
    D3D12_RASTERIZER_DESC RasterizerTwoSidedMsaa;   // カリング無効+マルチサンプルアンチエイリアシング
    D3D12_RASTERIZER_DESC RasterizerShadow;         // シャドウマッピング用
    D3D12_RASTERIZER_DESC RasterizerShadowCW;       // シャドウマッピング用+時計回りのポリゴンをカリング
    D3D12_RASTERIZER_DESC RasterizerShadowTwoSided; // シャドウマッピング用+カリング無効

    // BlendMode
    D3D12_BLEND_DESC NoneBlend;     // ブレンドなし
    D3D12_BLEND_DESC NormalBlend;   // 通常αブレンド
    D3D12_BLEND_DESC AddBlend;      // 加算ブレンド
    D3D12_BLEND_DESC SubtractBlend; // 減算ブレンド
    D3D12_BLEND_DESC MultilyBlend;  // 乗算ブレンド
    D3D12_BLEND_DESC ScreenBlend;   // スクリーンブレンド
    D3D12_BLEND_DESC blendDescs[BlendModeType::kBlendCount];


    // DepthState
    D3D12_DEPTH_STENCIL_DESC DepthStateDisabled;         // 無効
    D3D12_DEPTH_STENCIL_DESC DepthStateReadWrite;        // 通常
    D3D12_DEPTH_STENCIL_DESC DepthStateReadOnly;         // 読み取り専用
    D3D12_DEPTH_STENCIL_DESC DepthStateReadOnlyReversed; // 読み取り専用(逆順)
    D3D12_DEPTH_STENCIL_DESC DepthStateTestEqual;        // 等しい場合のみ

private:
    GraphicsCommon() = default;
    ~GraphicsCommon() = default;
    GraphicsCommon(const GraphicsCommon&) = delete;
    GraphicsCommon& operator=(const GraphicsCommon&) = delete;


};