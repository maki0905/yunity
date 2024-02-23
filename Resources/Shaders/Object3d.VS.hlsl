#include "Object3d.hlsli"
//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

struct WorldTransform
{
    float4x4 world;
    float4x4 worldInverseTranspose;
};
ConstantBuffer<WorldTransform> gWorldTransform : register(b0);

struct ViewProjection
{
    float4x4 view;
    float4x4 projection;
};
ConstantBuffer<ViewProjection> gViewProjection : register(b1);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
    output.position = mul(mul(input.position, gWorldTransform.world), mul(gViewProjection.view, gViewProjection.projection));
    //output.normal = normalize(mul(input.normal, (float3x3)gWorldTransform.world));
    output.normal = normalize(mul(input.normal, (float3x3) gWorldTransform.worldInverseTranspose));
    output.texcoord = input.texcoord;
    output.worldPosition = mul(input.position, gWorldTransform.world).xyz;
	return output;
}

