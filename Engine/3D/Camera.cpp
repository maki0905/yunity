#include "Camera.h"

#include "WindowsAPI.h"
#include <cassert>
#include "Device.h"
#include "Common.h"
#include "ImGuiManager.h"

//void Camera::Initialize() {
//    // 定数バッファの生成
//    //CreateConstBuffer();
//    // マッピング
//    Map();
//    // 行列の生成
//    UpdateMatrix();
//}

//void Camera::CreateConstBuffer() {
//    HRESULT result;
//
//    // ヒーププロパティ
//    //D3D12_HEAP_PROPERTIES heapProps = D3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
//    D3D12_HEAP_PROPERTIES heapProps{};
//    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
//    // リソース設定
//    D3D12_RESOURCE_DESC resourceDesc{};
//    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//    resourceDesc.Width = sizeof(ConstBufferDataViewProjection);
//    resourceDesc.Height = 1;
//    resourceDesc.DepthOrArraySize = 1;
//    resourceDesc.MipLevels = 1;
//    resourceDesc.SampleDesc.Count = 1;
//    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//    //CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff);
//    
//    // 定数バッファの生成
//    result = Device::GetInstance()->GetDevice()->CreateCommittedResource(
//        &heapProps, // アップロード可能
//        D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//        IID_PPV_ARGS(&constBuff_));
//    assert(SUCCEEDED(result));
//
//
//}

//void Camera::Map() {
//    constBuff_ = CreateBufferResource(sizeof(ConstBufferDataViewProjection));
//    cameraForGPU_ = CreateBufferResource(sizeof(CameraForGPU));
//
//    // 定数バッファとのデータリンク
//    HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap_);
//    assert(SUCCEEDED(result));
//
//    result = cameraForGPU_->Map(0, nullptr, (void**)&cameraForGPUMap_);
//    assert(SUCCEEDED(result));
//
//}
//
//void Camera::UpdateMatrix() {
//    // ビュー行列の生成
//    matView = MakeViewMatrix(rotation_, translation_);
//    //matView = MakeViewMatrix(quaternion_, translation_);
//
//    // 平行投影による射影行列の生成
//    // constMap->mat = XMMatrixOrthographicOffCenterLH(
//    //	0, window_width,
//    //	window_height, 0,
//    //	0, 1);
//    // 透視投影による射影行列の生成
//    matProjection = MakePerspectiveFovMatrix(fovAngleY, aspectRatio, nearZ, farZ);
//
//    // 定数バッファに書き込み
//    constMap_->view = matView;
//    constMap_->projection = matProjection;
//    
//    cameraForGPUMap_->worldPosition = translation_;
//
//}

Camera::Camera() :
	transform_({ {1.0f, 1.0f, 1.0f}, {/*std::numbers::pi_v<float> / 3.0f*/0, /*std::numbers::pi_v<float>*/0, 0 }, { 0, 10.0f, -50.0f} }),
	fovAngleY_(ConvertToRadians(45.0f)),
	aspectRatio_((float)16 / 9),
	nearClip_(10.0f),
	farClip_(1000.0f),
	worldMatrix_(MakeAffineMatrix(transform_)),
	viewMatrix_(Inverse(worldMatrix_)),
	projectionMatrix_(MakePerspectiveFovMatrix(fovAngleY_, aspectRatio_, nearClip_, farClip_))
{
	constBuff_ = CreateBufferResource(sizeof(ConstBufferDataViewProjection));
	constBuff_CameraForGPU_ = CreateBufferResource(sizeof(CameraForGPU));
	

	// 定数バッファとのデータリンク
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));

	result = constBuff_CameraForGPU_->Map(0, nullptr, (void**)&constMap_CameraForGPU_);
    assert(SUCCEEDED(result));

}

Vector3 Camera::Offset()
{
	Vector3 offset = { 0.0f, 5.0f, -50.0f };
	Matrix4x4 rotate = MakeRotateYMatrix(transform_.rotate.y);
	offset = TransformNormal(offset, rotate);
	return offset;
}

void Camera::Update()
{

	if (target_ != nullptr) {
		Vector3 interTarget = target_->GetMatWorldTranslation();
		if (fixedAxis_.x) {
			interTarget.x = 0.0f;
		}
		if (fixedAxis_.y) {
			interTarget.y = 0.0f;
		}
		if (fixedAxis_.z) {
			interTarget.z = 0.0f;
		}

		if(!fixedAngle_.x){
			transform_.rotate.x = target_->rotation_.x;
		}
		if (!fixedAngle_.y) {
			transform_.rotate.y = target_->rotation_.y;
		}
		if (!fixedAngle_.z) {
			transform_.rotate.z = target_->rotation_.z;
		}

		Vector3 offset = Offset();

		transform_.translate = Add(interTarget, offset);
	}
	worldMatrix_ = MakeAffineMatrix(transform_);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovAngleY_, aspectRatio_, nearClip_, farClip_);

	// 定数バッファに書き込み
	constMap_->view = viewMatrix_;
	constMap_->projection = projectionMatrix_;

	//constMap_CameraForGPU_->worldPosition = { worldMatrix_.m[3][0], worldMatrix_.m[3][1], worldMatrix_.m[3][2] };
	constMap_CameraForGPU_->worldPosition = transform_.translate;
}


