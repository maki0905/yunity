#include "Particle.hlsli"

struct ParticleForGPU
{
    float4x4 world;
};
//ConstantBuffer<WorldTransform> gWorldTransform : register(b0);

StructuredBuffer<ParticleForGPU> gParticleForGPU : register(t0);

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

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = mul(mul(input.position, gParticleForGPU[instanceId].world), mul(gViewProjection.view, gViewProjection.projection));
    output.normal = normalize(mul(input.normal, (float3x3) gParticleForGPU[instanceId].world));
    //output.position = mul(mul(input.position, gWorldTransform.world), mul(gViewProjection.view, gViewProjection.projection));
    //output.normal = normalize(mul(input.normal, (float3x3) gWorldTransform.world));
    output.texcoord = input.texcoord;
    return output;
}

