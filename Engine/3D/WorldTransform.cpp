#include "WorldTransform.h"
#include <cassert>

#include "Device.h"

void WorldTransform::Initialize() {
    CreateConstBuffer();
    Map();
    matWorld_ = MakeIdentity4x4();
    matWorldInverseTranspose_ = Transpose(Inverse(matWorld_));
    TransferMatrix();
    quaternion_ = IndentityQuaternion();
}

void WorldTransform::CreateConstBuffer() {
    HRESULT result;

    // ヒーププロパティ
    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    // リソース設定
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = sizeof(ConstBufferDataWorldTransform);
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff);

    // 定数バッファの生成
    result = Device::GetInstance()->GetDevice()->CreateCommittedResource(
        &heapProps, // アップロード可能
        D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&constBuff_));
    assert(SUCCEEDED(result));
}

void WorldTransform::Map() {
    // 定数バッファとのデータリンク
    HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result));
}

void WorldTransform::UpdateMatrix(RotationType type) {

    switch (type)
    {
    case RotationType::Euler:
        // スケール、回転、平行移動を合成して行列を計算する
        matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
        break;
    case RotationType::Quaternion:
        // スケール、回転、平行移動を合成して行列を計算する
        matWorld_ = MakeAffineMatrix(scale_,quaternion_, translation_);
        break;
    }

    // 親があれば親のワールド行列を掛ける
    if (parent_) {
        matWorld_ = Multiply(matWorld_, parent_->matWorld_);
    }

    matWorldInverseTranspose_ = Transpose(Inverse(matWorld_));

    TransferMatrix();
}

void WorldTransform::TransferMatrix()
{
    // 定数バッファに書き込み
    constMap->matWorld = matWorld_;
    constMap->matWorldInverseTranspose = matWorldInverseTranspose_;
}
