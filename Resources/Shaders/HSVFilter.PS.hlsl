#include "Interconversion.hlsli"
#include "Fullscreen.hlsli"

struct Material
{
    float4x4 projectionInverse;
    float32_t hue;
    float32_t saturation;
    float32_t value;
};


Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

float32_t WrapValue(float32_t value, float32_t minRange, float32_t maxRange)
{
    float32_t range = maxRange - minRange;
    float32_t modValue = fmod(value - minRange, range);
    if (modValue < 0)
    {
        modValue += range;
    }
    return minRange + modValue;
}

PixelShaderOutput main(VertexShaerOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float32_t3 hsv = RGBToHSV(textureColor.rgb);
    
    // Т▓Ро
    hsv.x += gMaterial.hue;
    hsv.y += gMaterial.saturation;
    hsv.z += gMaterial.value;
    hsv.x = WrapValue(hsv.x, 0.0f, 1.0f);
    hsv.y = saturate(hsv.y);
    hsv.z = saturate(hsv.z);
    
    float32_t3 rgb = HSVToRGB(hsv);
    
    output.color.rgb = rgb;
    output.color.a = textureColor.a;
    
    return output;
}