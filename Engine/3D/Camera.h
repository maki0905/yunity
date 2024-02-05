#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "Transform.h"
#include "MathFunction.h"

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4x4 view;       // ワールド → ビュー変換行列
	Matrix4x4 projection; // ビュー → プロジェクション変換行列
};

struct CameraForGPU {
	Vector3 worldPosition;
};

 ///<summary>
 ///ビュープロジェクション変換データ
 ///</summary>
struct Camera {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGPU_;
	CameraForGPU* cameraForGPUMap_ = nullptr;

#pragma region ビュー行列の設定
	// ローカル座標
	Vector3 translation_ = { 0, 10.0f, -50.0f }; // 座標
	// 回転
	Vector3 rotation_ = { /*std::numbers::pi_v<float> / 3.0f*/0, /*std::numbers::pi_v<float>*/0, 0 }; // 回転
	Quaternion quaternion_ = {0.0f, 0.0f, 0.0f, 1.0f}; // クォータニオン
	
#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = ConvertToRadians(45.0f);
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 10.0f;
	// 深度限界（奥側）
	float farZ = 1000.0f;
#pragma endregion

	// ビュー行列
	Matrix4x4 matView;
	// 射影行列
	Matrix4x4 matProjection;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	//void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();

	void SetTranslation(const Vector3 translation) { translation_ = translation; }
	void SetRotation(const Vector3 rotation) { rotation_ = rotation; }
};

//class Camera {
//public:
//
//	Camera();
//
//	void Update();
//
//	// setter
//	void SetRotate(const Vector3& rotate) { m_Transform.rotate = rotate; }
//	void SetTranslate(const Vector3& translate) { m_Transform.translate = translate; }
//	void SetFovY(float fovY) { m_FovAngleY = fovY;}
//	void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
//	void SetNearClip(float nearClip) { m_NearClip = nearClip; }
//	void SetFarClip(float farClip) { m_FarClip = farClip; }
//
//	// getter
//	ID3D12Resource* GetConstBuff() const { return p_ConstBuff.Get(); }
//	const Matrix4x4& GetWorldMatrix() const { return m_WorldMatrix; }
//	const Matrix4x4& GetViewMatrix() const { return m_ViewMatrix; }
//	const Matrix4x4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
//	const Vector3& GetRotate()const { return m_Transform.rotate; }
//	const Vector3& GetTranslate()const { return m_Transform.translate; }
//
//private:
//	// 定数バッファ
//	Microsoft::WRL::ComPtr<ID3D12Resource> p_ConstBuff;
//	// マッピング済みアドレス
//	ConstBufferDataViewProjection* p_ConstMap = nullptr;
//
//	Matrix4x4 m_WorldMatrix;
//	Matrix4x4 m_ViewMatrix;
//	Matrix4x4 m_ProjectionMatrix;
//
//#pragma region ビュー行列の設定
//	Transform m_Transform;
//#pragma endregion
//
//#pragma region 射影行列の設定
//	// 垂直方向視野角
//	float m_FovAngleY = ConvertToRadians(45.0f);
//	// ビューポートのアスペクト比
//	float m_AspectRatio = (float)16 / 9;
//	// ニアクリップ距離
//	float m_NearClip = 10.0f;
//	// ファークリップ距離
//	float m_FarClip = 1000.0f;
//#pragma endregion
//};