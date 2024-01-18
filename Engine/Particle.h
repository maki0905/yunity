#pragma once

#include "Vector3.h"

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct Particle {
	Transform transfrom;
	Vector3 velocity;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
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
	//void UpdateMatrix(RotationType type);
	/// <summary>
	/// 定数バッファに転送する
	/// </summary>
	void TransferMatrix();

};


