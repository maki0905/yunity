#include "GraphicsCommon.h"



yunity::GraphicsCommon* yunity::GraphicsCommon::GetInstance()
{
    static GraphicsCommon instance;

    return &instance;
}

void yunity::GraphicsCommon::Initialize()
{
    D3D12_STATIC_SAMPLER_DESC staticSamplerDesc;
    staticSamplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
    staticSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplerDesc.MipLODBias = 0.0f;
    staticSamplerDesc.MaxAnisotropy = 16;
    staticSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    staticSamplerDesc.MinLOD = 0.0f;
    staticSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

    StaticSampler = staticSamplerDesc;
    StaticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

    // Default rasterizer states
    RasterizerDefault = {};
    RasterizerDefault.FillMode = D3D12_FILL_MODE_SOLID;
    RasterizerDefault.CullMode = D3D12_CULL_MODE_BACK;

    RasterizerDefaultMsaa = RasterizerDefault;
    RasterizerDefaultMsaa.MultisampleEnable = TRUE;

    RasterizerDefaultCw = RasterizerDefault;
    RasterizerDefaultCw.FrontCounterClockwise = FALSE;

    RasterizerDefaultCwMsaa = RasterizerDefaultCw;
    RasterizerDefaultCwMsaa.MultisampleEnable = TRUE;

    RasterizerTwoSided = RasterizerDefault;
    RasterizerTwoSided.CullMode = D3D12_CULL_MODE_NONE;

    RasterizerTwoSidedMsaa = RasterizerTwoSided;
    RasterizerTwoSidedMsaa.MultisampleEnable = TRUE;

    // Shadows need their own rasterizer state so we can reverse the winding of faces
    RasterizerShadow = RasterizerDefault;
    //RasterizerShadow.CullMode = D3D12_CULL_FRONT;  // Hacked here rather than fixing the content
    RasterizerShadow.SlopeScaledDepthBias = -1.5f;
    RasterizerShadow.DepthBias = -100;

    RasterizerShadowTwoSided = RasterizerShadow;
    RasterizerShadowTwoSided.CullMode = D3D12_CULL_MODE_NONE;

    RasterizerShadowCW = RasterizerShadow;
    RasterizerShadowCW.FrontCounterClockwise = FALSE;

    D3D12_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    NoneBlend = blendDesc;

    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    NormalBlend = blendDesc;

    AddBlend = blendDesc;
    AddBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    AddBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    AddBlend.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

    SubtractBlend = blendDesc;
    SubtractBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    SubtractBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
    SubtractBlend.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

    MultilyBlend = blendDesc;
    MultilyBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
    MultilyBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    MultilyBlend.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;

    ScreenBlend = blendDesc;
    ScreenBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
    ScreenBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    ScreenBlend.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

    blendDescs[BlendModeType::kNone] = NoneBlend;
    blendDescs[BlendModeType::kNormal] = NormalBlend;
    blendDescs[BlendModeType::kAdd] = AddBlend;
    blendDescs[BlendModeType::kSubtract] = SubtractBlend;
    blendDescs[BlendModeType::kMultily] = MultilyBlend;
    blendDescs[BlendModeType::kScreen] = ScreenBlend;

    DepthStateDisabled.DepthEnable = FALSE;
    DepthStateDisabled.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    DepthStateDisabled.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    DepthStateDisabled.StencilEnable = FALSE;
    DepthStateDisabled.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    DepthStateDisabled.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    DepthStateDisabled.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    DepthStateDisabled.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    DepthStateDisabled.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    DepthStateDisabled.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    DepthStateDisabled.BackFace = DepthStateDisabled.FrontFace;

    DepthStateReadWrite =  DepthStateDisabled;
    DepthStateReadWrite.DepthEnable = TRUE;
    DepthStateReadWrite.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    DepthStateReadWrite.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

    DepthStateReadOnly = DepthStateReadWrite;
    DepthStateReadOnly.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

    DepthStateReadOnlyReversed = DepthStateReadOnly;
    DepthStateReadOnlyReversed.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

    DepthStateTestEqual = DepthStateReadOnly;
    DepthStateTestEqual.DepthFunc = D3D12_COMPARISON_FUNC_EQUAL;
}
