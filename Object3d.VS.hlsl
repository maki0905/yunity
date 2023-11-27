#include "Object3d.hlsli"
//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

struct WorldTransform
{
    float32_t4x4 world;
};
ConstantBuffer<WorldTransform> gWorldTransform : register(b0);

struct ViewProjection
{
    float32_t4x4 view;
    float32_t4x4 projection;
};
ConstantBuffer<ViewProjection> gViewProjection : register(b1);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
    output.position = mul(mul(input.position, gWorldTransform.world), mul(gViewProjection.view, gViewProjection.projection));
    output.normal = normalize(mul(input.normal, (float32_t3x3)gWorldTransform.world));
    output.texcoord = input.texcoord;
	return output;
}

