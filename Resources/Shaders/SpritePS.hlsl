
cbuffer cbuff0 : register(b0)
{
    float4 color; // 色(RGBA)
    matrix mat; // 3D変換行列
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
};

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_Target
{
    return tex.Sample(smp, input.uv) * color;
}