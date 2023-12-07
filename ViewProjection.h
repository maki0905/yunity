//#pragma once
//
//#include <d3d12.h>
//#include <wrl.h>
//#include "MathFunction.h"
//
//// 定数バッファ用データ構造体
//struct ConstBufferDataViewProjection {
//	Matrix4x4 view;       // ワールド → ビュー変換行列
//	Matrix4x4 projection; // ビュー → プロジェクション変換行列
//};
//
///// <summary>
///// ビュープロジェクション変換データ
///// </summary>
//struct ViewProjection {
//	// 定数バッファ
//	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
//	// マッピング済みアドレス
//	ConstBufferDataViewProjection* constMap_ = nullptr;
//
//#pragma region ビュー行列の設定
//	// ローカル座標
//	Vector3 translation_ = { 0, 10.0f, -50.0f }; // 座標
//	// 回転
//	Vector3 rotation_ = { 0, 0, 0 }; // 回転
//	Quaternion quaternion_ = {0.0f, 0.0f, 0.0f, 1.0f}; // クォータニオン
//	
//#pragma endregion
//
//#pragma region 射影行列の設定
//	// 垂直方向視野角
//	float fovAngleY = ConvertToRadians(45.0f);
//	// ビューポートのアスペクト比
//	float aspectRatio = (float)16 / 9;
//	// 深度限界（手前側）
//	float nearZ = 10.0f;
//	// 深度限界（奥側）
//	float farZ = 1000.0f;
//#pragma endregion
//
//	// ビュー行列
//	Matrix4x4 matView;
//	// 射影行列
//	Matrix4x4 matProjection;
//
//	/// <summary>
//	/// 初期化
//	/// </summary>
//	void Initialize();
//	/// <summary>
//	/// 定数バッファ生成
//	/// </summary>
//	void CreateConstBuffer();
//	/// <summary>
//	/// マッピングする
//	/// </summary>
//	void Map();
//	/// <summary>
//	/// 行列を更新する
//	/// </summary>
//	void UpdateMatrix();
//};
