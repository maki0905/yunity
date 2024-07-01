#include "Primitive.hlsli"

struct WorldTransform
{
    float4x4 world;
};
ConstantBuffer<WorldTransform> gWorldTransform : register(b0);

struct ViewProjection
{
    matrix view; // ビュー変換行列
    matrix projection; // プロジェクション変換行列
};

ConstantBuffer<ViewProjection> gViewProjection : register(b1);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float4 color : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(mul(input.position, gWorldTransform.world), mul(gViewProjection.view, gViewProjection.projection));
    output.color = input.color;
    return output;
}