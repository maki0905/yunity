#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "Transform.h"
#include "MathFunction.h"
#include "WorldTransform.h"

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4x4 view;       // ワールド → ビュー変換行列
	Matrix4x4 projection; // ビュー → プロジェクション変換行列
};

struct CameraForGPU {
	Vector3 worldPosition;
};

/*
* @brief カメラクラス
*/
class Camera {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// setter
	/// </summary>
	
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	void SetFovY(float fovY) { fovAngleY_ = fovY;}
	void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
	void SetNearClip(float nearClip) { nearClip_ = nearClip; }
	void SetFarClip(float farClip) { farClip_ = farClip; }
	void SetTarget(WorldTransform* target) { target_ = target; }
	void SetFixedAngle(const Vector3& fixedAngle) { fixedAngle_ = fixedAngle; }
	void SetFixedAxis(const Vector3& fixedAxis) { fixedAxis_ = fixedAxis; }
	void SetOffset(const Vector3& offset) { offset_ = offset; }

	/// <summary>
	/// getter
	/// </summary>
	/// 
	ID3D12Resource* GetConstBuff() const { return constBuff_.Get(); }
	ID3D12Resource* GetCameraForGPU() const { return constBuff_CameraForGPU_.Get(); }
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Vector3& GetRotate()const { return transform_.rotate; }
	const Vector3& GetTranslate()const { return transform_.translate; }
	WorldTransform* GetTarget() { return target_; }
	Vector3 GetFixedAngle() { return fixedAngle_; }
	Vector3 GetFixedAxis() { return fixedAxis_; }
	Vector3 GetOffset() { return offset_; }
private:
	/// <summary>
	/// オフセット
	/// </summary>
	/// <returns>オフセット</returns>
	Vector3 Offset();

private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_CameraForGPU_;
	CameraForGPU* constMap_CameraForGPU_ = nullptr;

	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;

	WorldTransform* target_;
	Vector3 fixedAngle_;
	Vector3 fixedAxis_;
	Vector3 offset_;

#pragma region ビュー行列の設定
	EulerTransform transform_;
#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY_ = ConvertToRadians(45.0f);
	// ビューポートのアスペクト比
	float aspectRatio_ = (float)16 / 9;
	// ニアクリップ距離
	float nearClip_ = 10.0f;
	// ファークリップ距離
	float farClip_ = 1000.0f;
#pragma endregion
};