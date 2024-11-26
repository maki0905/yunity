#include "Particle.hlsli"

struct Material
{
    float4 color;
    //int32_t enableLighting;
};


//ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);


struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    output.color = gMaterial.color * textureColor;
    if (textureColor.a <= 0.1)
    {
        discard;
    }
    output.color = gMaterial.color * textureColor * input.color;
    
    return output;
}

