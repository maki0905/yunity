#include "Camera.h"

#include "WindowsAPI.h"
#include <cassert>
#include "Device.h"
#include "Common.h"
#include "ImGuiManager.h"

yunity::Camera::Camera() :
	transform_({ {1.0f, 1.0f, 1.0f}, {0, 0, 0 }, { 0, 10.0f, -50.0f} }),
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

	offset_ = { 0.0f, 5.0f, -50.0f };
}

Vector3 yunity::Camera::Offset()
{
	Vector3 offset = { 0.0f, 5.0f, -50.0f };
	Matrix4x4 rotate = MakeRotateYMatrix(transform_.rotate.y);
	offset = TransformNormal(offset_, rotate);
	return offset;
}

void yunity::Camera::Update()
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
	constMap_CameraForGPU_->worldPosition = transform_.translate;
}


