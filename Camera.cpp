﻿#include "Camera.h"

#include "WindowsAPI.h"
#include <cassert>
#include "Device.h"

void Camera::Initialize() {
    // 定数バッファの生成
    CreateConstBuffer();
    // マッピング
    Map();
    // 行列の生成
    UpdateMatrix();
}

void Camera::CreateConstBuffer() {
    HRESULT result;

    // ヒーププロパティ
    //D3D12_HEAP_PROPERTIES heapProps = D3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    // リソース設定
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = sizeof(ConstBufferDataViewProjection);
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff);
    
    // 定数バッファの生成
    result = Device::GetInstance()->GetDevice()->CreateCommittedResource(
        &heapProps, // アップロード可能
        D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&constBuff_));
    assert(SUCCEEDED(result));
}

void Camera::Map() {
    // 定数バッファとのデータリンク
    HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap_);
    assert(SUCCEEDED(result));
}

void Camera::UpdateMatrix() {
    // ビュー行列の生成
    matView = MakeViewMatrix(rotation_, translation_);
    //matView = MakeViewMatrix(quaternion_, translation_);

    // 平行投影による射影行列の生成
    // constMap->mat = XMMatrixOrthographicOffCenterLH(
    //	0, window_width,
    //	window_height, 0,
    //	0, 1);
    // 透視投影による射影行列の生成
    matProjection = MakePerspectiveFovMatrix(fovAngleY, aspectRatio, nearZ, farZ);

    // 定数バッファに書き込み
    constMap_->view = matView;
    constMap_->projection = matProjection;
}
