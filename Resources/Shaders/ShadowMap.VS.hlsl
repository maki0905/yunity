#include "Object3DShadowMap.hlsli"

ConstantBuffer<WorldTransform> gWorldTransform : register(b0);
ConstantBuffer<ViewProjection> gViewProjection : register(b1);
//シャドーマップ計算用頂点シェーダ
float4 main(VertexShaderInput input) : SV_POSITION
{
    //float4 pos = { 0.0f, 0.0f, 0.0f, 0.0f };
    float4 pos = input.position;
    pos.w = 1.0f;
    pos = mul(pos, gWorldTransform.world);
    pos = mul(pos, mul(gViewProjection.view, gViewProjection.projection));
    
    return pos;
}
