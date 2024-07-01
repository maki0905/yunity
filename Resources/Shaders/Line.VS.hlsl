#include "Line.hlsli"

struct ViewProjection
{
    matrix view; // ビュー変換行列
    matrix projection; // プロジェクション変換行列
};
ConstantBuffer<ViewProjection> gViewProjection : register(b0);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t4 color : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, mul(gViewProjection.view, gViewProjection.projection));
    output.color = input.color;
    return output;
}