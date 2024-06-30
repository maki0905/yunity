#include "Object3d.hlsli"

struct Material
{
    float32_t4 color;
    int enableLighting;
    float shininees;
};

struct DirectionalLight
{
    float32_t4 color; // ライトの色
    float32_t3 direction; // ライトの向き
    float intensity; // 輝度
};

struct Camera
{
    float3 worldPosition;
};

struct PointLight
{
    float32_t4 color; // ライトの色
    float32_t3 position; // ライトの位置
    float intensity; // 輝度
    float radius; // 半径
    float decay; // 減衰率
};

//ConstantBuffer<Material> gMaterial : register(b0);
    
Texture2D<float32_t4> gTexture : register(t0);
TextureCube<float32_t4> gEnvironmentTexture : register(t1);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);


struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
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
        
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    
    float distance = length(gPointLight.position - input.worldPosition);
    //float d2 = distance * distance;
    //float r2 = gPointLight.radius * gPointLight.radius;
    //float factor = 2.0f / d2 + r2 + distance * sqrt(d2 + r2);
    
    //float s = distance / gPointLight.radius;
    //float factor = 0.0f;
    
    //if (s < 1.0f)
    //{
    //    float s2 = s * s;
        
    //    factor = gPointLight.intensity * pow((1.0f - s2), 2) / (1.0f + gPointLight.decay * s);
    //}
    
    //float factor = 1.0f / (distance * distance);
    float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
    
    
    float32_t3 pointLightdirection = normalize(input.worldPosition - gPointLight.position);
    float32_t4 pointLightColor = gPointLight.color;
    pointLightColor.rgb = gPointLight.color.rgb * gPointLight.intensity * factor;
        
    // Phong Reflection
    //float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    //float RdotE = dot(reflectLight, toEye);
    //float specularPow = pow(saturate(RdotE), gMaterial.shininees);
    
    // Blinn-Phong Reflection
    float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
    float NDotH = dot(normalize(input.normal), halfVector);
    float specularPow = pow(saturate(NDotH), gMaterial.shininees);
    
    float3 halfVector_PointLight = normalize(-pointLightdirection + toEye);
    float NDotH_PointLight = dot(normalize(input.normal), halfVector_PointLight);
    float specularPow_PointLight = pow(saturate(NDotH_PointLight), gMaterial.shininees);
    
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
        // 拡散反射
        float3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * pointLightColor.rgb * cos * gPointLight.intensity;
        // 鏡面反射
        float3 specularDirectionalLight = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
        float3 specularPointLight = pointLightColor.rgb * gPointLight.intensity * specularPow_PointLight * float3(1.0f, 1.0f, 1.0f);
        
        output.color.rgb = diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    //float32_t3 cameraToPosition = normalize(input.worldPosition - gCamera.worldPosition);
    float32_t3 cameraToPosition = normalize(gCamera.worldPosition - input.worldPosition);
    float32_t3 reflectedVector = reflect(cameraToPosition, normalize(input.normal));
    float32_t4 environamentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
    
    //output.color.rgb = environamentColor.rgb;
    
    return output;
}

