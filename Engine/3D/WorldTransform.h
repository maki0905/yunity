#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "MathFunction.h"

namespace yunity {
	// 定数バッファ用データ構造体
	struct ConstBufferDataWorldTransform {
		Matrix4x4 matWorld; // ローカル → ワールド変換行列
		Matrix4x4 matWorldInverseTranspose; // ワールドの逆転置行列
	};

	enum class RotationType
	{
		Euler,
		Quaternion
	};

	/// <summary>
	/// ワールド変換データ
	/// </summary>
	struct WorldTransform {
		// 定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
		// マッピング済みアドレス
		ConstBufferDataWorldTransform* constMap = nullptr;
		// ローカルスケール
		Vector3 scale_ = { 1, 1, 1 };
		// X,Y,Z軸回りのローカル回転角
		Vector3 rotation_ = { 0, 0, 0 };
		Quaternion quaternion_;
		// ローカル座標
		Vector3 translation_ = { 0, 0, 0 };
		// ローカル → ワールド変換行列
		Matrix4x4 matWorld_;
		// 親となるワールド変換へのポインタ
		WorldTransform* parent_ = nullptr;
		// ワールドの逆転置行列
		Matrix4x4 matWorldInverseTranspose_;
		// ローテートタイプ
		RotationType rotateType_;


		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(RotationType rotateType = RotationType::Euler);
		/// <summary>
		/// 定数バッファ生成
		/// </summary>
		void CreateConstBuffer();
		/// <summary>
		/// マッピングする
		/// </summary>
		void Map();
		/// <summary>
		/// 行列を更新する
		/// </summary>
		void UpdateMatrix();
		/// <summary>
		/// 定数バッファに転送する
		/// </summary>
		void TransferMatrix();
		/// <summary>
		/// ワールド座標系での位置を取得
		/// </summary>
		/// <returns>Vector3</returns>
		Vector3 GetMatWorldTranslation() { return { matWorld_.m[3][0], matWorld_.m[3][1], matWorld_.m[3][2] }; }
	};


}
