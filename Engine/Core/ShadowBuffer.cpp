#include "ShadowBuffer.h"



yunity::ShadowBuffer* yunity::ShadowBuffer::GetInstance()
{
    static ShadowBuffer instance;
    return &instance;
}

void yunity::ShadowBuffer::Initalize()
{
    shadowBuffer_ = std::make_unique<DepthBuffer>();
    shadowBuffer_->Initialize(DXGI_FORMAT_D32_FLOAT);

}
