#include "Fullscreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
Texture2D<float32_t> gDepthTexture : register(t1);
SamplerState gSamplerPoint : register(s1);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f, }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float32_t kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float32_t kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

float32_t Luminance(float32_t3 v)
{
    return dot(v, float32_t3(0.2125f, 0.7154f, 0.0721f));
}

PixelShaderOutput main(VertexShaerOutput input)
{
    uint32_t width;
    uint32_t height;
    
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
    float32_t2 difference = float32_t2(0.0f, 0.0f); // �c�����ꂼ��̏�ݍ��݂̌��ʂ��i�[����
    // �F���P�x�ɕϊ����āA��ݍ��݂��s���Ă����B����Filter�p��kernel�ɂȂ��Ă���̂ŁA��邱�Ǝ��͍̂��܂ł̏�ݍ��݂Ɠ���
    for (int32_t x = 0; x < 3; ++x)
    {
        for (int32_t y = 0; y < 3; ++y)
        {
            float32_t2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            float32_t ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
            // NDC -> View�BP^{-1}�ɂ������I�Ƃ���zw�ɉe����^���Ȃ̂łȂ�ł��ǂ��B�Ȃ̂ŁA�킴�킴�s���n���Ȃ��Ă��ǂ�
            // gMaterial.projectionInverse��CBuffer���g���ēn���Ă�������
            float32_t4 viewSpace = mul(float32_t4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
            float32_t viewZ = viewSpace.z * rcp(viewSpace.w); // �������W�n����f�J���g���W�n�֕ϊ�
            difference.x += viewZ * kPrewittHorizontalKernel[x][y];
            difference.y += viewZ * kPrewittVerticalKernel[x][y];
            
        }
    }
    
    // �ω��̒������E�F�C�g�Ƃ��č����B�E�F�C�g�̌�����@���F�X�l������B���Ƃ���difference.x�����g���Ή������̃G�b�W�����o�����
    float32_t weight = length(difference);
    
    weight = saturate(weight);
    
    PixelShaderOutput output;
    // wight���傫���قƈÂ��\������悤�ɂ��Ă���B�ł��V���v���ȍ������@
    output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler, input.texcoord).rgb;
    output.color.a = 1.0f;
    
    return output;
}