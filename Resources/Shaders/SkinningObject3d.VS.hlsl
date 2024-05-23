#include "Object3d.hlsli"

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

struct Node
{
    float4x4 localMatrix;
};
ConstantBuffer<Node> gNode : register(b2);

struct Well
{
    float32_t4x4 skeletonSpaceMatrix;
    float32_t4x4 skeletonSpaceInverseTransposeMatrix;
};
StructuredBuffer<Well> gMatrixPalette : register(t0);

struct Skinned
{
    float32_t4 postion;
    float32_t3 normal;
};

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t4 weight : WEIGHT0;
    int32_t4 index : INDEX0;
};

Skinned Skinning(VertexShaderInput input)
{
    Skinned skinned;
    skinned.postion = mul(input.position, gMatrixPalette[input.index.x].skeletonSpaceMatrix) * input.weight.x;
    skinned.postion += mul(input.position, gMatrixPalette[input.index.y].skeletonSpaceMatrix) * input.weight.y;
    skinned.postion += mul(input.position, gMatrixPalette[input.index.z].skeletonSpaceMatrix) * input.weight.z;
    skinned.postion += mul(input.position, gMatrixPalette[input.index.w].skeletonSpaceMatrix) * input.weight.w;
    skinned.postion.w = 1.0f; // 確実に1を入れる
    
    // 法線の変換
    skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.x].skeletonSpaceInverseTransposeMatrix) * input.weight.x;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.y].skeletonSpaceInverseTransposeMatrix) * input.weight.y;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.z].skeletonSpaceInverseTransposeMatrix) * input.weight.z;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.w].skeletonSpaceInverseTransposeMatrix) * input.weight.w;
    skinned.normal = normalize(skinned.normal); // 正規化して戻してあげる
    return skinned;
}

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    Skinned skinned = Skinning(input); // まずSkinning計算を行って、Skinning後の頂点情報を手に入れる。ここでの頂点もSkeletonSpace
    //float4x4 world = mul(gNode.localMatrix, gWorldTransform.world);
    float4x4 wvp = mul(gWorldTransform.world, mul(gViewProjection.view, gViewProjection.projection));
    //output.position = mul(mul(input.position, gWorldTransform.world), mul(gViewProjection.view, gViewProjection.projection));
    //output.position = mul(mul(input.position, world), mul(gViewProjection.view, gViewProjection.projection));
    output.position = mul(skinned.postion, wvp);
    output.worldPosition = mul(skinned.postion, gWorldTransform.world).xyz;
    //output.normal = normalize(mul(input.normal, (float3x3)gWorldTransform.world));
    output.normal = normalize(mul(skinned.normal, (float3x3) gWorldTransform.worldInverseTranspose));
    output.texcoord = input.texcoord;
    //output.worldPosition = mul(input.position, gWorldTransform.world).xyz;
    return output;
}

