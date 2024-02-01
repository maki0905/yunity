#include "Object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float shininees;
};

struct DirectionalLight
{
    float4 color; // ライトの色
    float3 direction; // ライトの向き
    float intensity; // 輝度
};

struct Camera
{
    float3 worldPosition;
};

//ConstantBuffer<Material> gMaterial : register(b0);
    
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);


struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	//PixelShaderOutput output;
 //   float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
 //   float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
 //   float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
 //   if (textureColor.a <= 0.5)
 //   {
 //       discard;
 //   }
    
 //   if (textureColor.a == 0.0)
 //   {
 //       discard;
 //   }
    
 //   if (output.color.a == 0.0)
 //   {
 //       discard;
 //   }
 //   output.color.rgb =   gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
 //   output.color.a = gMaterial.color.a * textureColor.a;
    
    PixelShaderOutput output;
    
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    // Phong Reflection
    //float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    //float RdotE = dot(reflectLight, toEye);
    //float specularPow = pow(saturate(RdotE), gMaterial.shininees);
    
    // Blinn-Phong Reflection
    float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
    float NDotH = dot(normalize(input.normal), halfVector);
    float specularPow = pow(saturate(NDotH), gMaterial.shininees);
    
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        if (textureColor.a <= 0.5)
        {
            discard;
        }
    
        if (textureColor.a == 0.0)
        {
            discard;
        }
    
        if (output.color.a == 0.0)
        {
            discard;
        }
        // 拡散反射
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        // 鏡面反射
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    
    
	return output;
}

