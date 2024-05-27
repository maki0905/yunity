#include "Fullscreen.hlsli"

struct Material
{
    float4x4 projectionInverse;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
Texture2D<float> gDepthTexture : register(t1);
SamplerState gSamplerPoint : register(s1);
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

static const float2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f, }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

float Luminance(float3 v)
{
    return dot(v, float3(0.2125f, 0.7154f, 0.0721f));
}

PixelShaderOutput main(VertexShaerOutput input)
{
    uint width;
    uint height;
    
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp(width), rcp(height));
    
    float2 difference = float2(0.0f, 0.0f); // �c�����ꂼ��̏�ݍ��݂̌��ʂ��i�[����
    // �F���P�x�ɕϊ����āA��ݍ��݂��s���Ă����B����Filter�p��kernel�ɂȂ��Ă���̂ŁA��邱�Ǝ��͍̂��܂ł̏�ݍ��݂Ɠ���
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            float ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
            // NDC -> View�BP^{-1}�ɂ������I�Ƃ���zw�ɉe����^���Ȃ̂łȂ�ł��ǂ��B�Ȃ̂ŁA�킴�킴�s���n���Ȃ��Ă��ǂ�
            // gMaterial.projectionInverse��CBuffer���g���ēn���Ă�������
            float4 viewSpace = mul(float4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
            float viewZ = viewSpace.z * rcp(viewSpace.w); // �������W�n����f�J���g���W�n�֕ϊ�
            difference.x += viewZ * kPrewittHorizontalKernel[x][y];
            difference.y += viewZ * kPrewittVerticalKernel[x][y];
            
        }
    }
    
    // �ω��̒������E�F�C�g�Ƃ��č����B�E�F�C�g�̌�����@���F�X�l������B���Ƃ���difference.x�����g���Ή������̃G�b�W�����o�����
    float weight = length(difference);
    
    weight = saturate(weight);
    
    PixelShaderOutput output;
    // wight���傫���قƈÂ��\������悤�ɂ��Ă���B�ł��V���v���ȍ������@
    output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler, input.texcoord).rgb;
    output.color.a = 1.0f;
    
    return output;
}