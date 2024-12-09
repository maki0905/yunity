#include "Object3DShadowMap.hlsli"

ConstantBuffer<WorldTransform> gWorldTransform : register(b0);
ConstantBuffer<ViewProjection> gViewProjection : register(b1);
ConstantBuffer<ViewProjection> gLightViewProjection : register(b3);

struct Node
{
    float4x4 localMatrix;
};
ConstantBuffer<Node> gNode : register(b2);

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(mul(input.position, gWorldTransform.world), mul(gViewProjection.view, gViewProjection.projection));
    output.normal = normalize(mul(input.normal, (float3x3) gWorldTransform.worldInverseTranspose));
    output.texcoord = input.texcoord;
    output.worldPosition = mul(input.position, gWorldTransform.world).xyz;
    
    float32_t4 pos = float32_t4(input.position.xyz, 1.0f);
    pos = mul(pos, gWorldTransform.world);
    pos = mul(pos, mul(gLightViewProjection.view, gLightViewProjection.projection));
    pos.xyz = pos.xyz / pos.w;
    output.shadowMapPosition.x = (1.0f + pos.x) / 2.0f;
    output.shadowMapPosition.y = (1.0f - pos.y) / 2.0f;
    output.shadowMapPosition.z = pos.z;
    return output;
}