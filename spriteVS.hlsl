
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

VSOutput main(float4 pos : POSITIONT, float2 uv : TEXCOORD)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.svpos = mul(mat, pos);
    output.uv = uv;
    return output;
}