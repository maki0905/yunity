#pragma once

#include "Object3D.h"
#include "Input.h"

/*
* @brief セレクト用のTVクラス
*/
class SelectTV : public yunity::Object3D {
	struct TVConstant {
		const Vector3 translation = { 80.0f, 18.0f, 12.0f };
		const float addition = 30.0f;
		const float rotationX = -15.0f * DegToRad();
	};

	struct Easing {
		bool flag;
		float t;
		Vector3 scale;
	};

	enum class SelectEvent { GROW, SHRINK, ZoomIn, ZoomOut, COUNT };
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突時に呼ばれる固有の処理
	/// </summary>
	void OnTriggerEvent() override;
private:
	/// <summary>
	/// TV拡大
	/// </summary>
	void Grow();

	/// <summary>
	/// TV縮小
	/// </summary>
	void Shrink();

	/// <summary>
	/// カメラズームイン
	/// </summary>
	void ZoomIn();

	/// <summary>
	/// カメラズームアウト
	/// </summary>
	void ZoomOut();

private:
	bool isActiveTV_;
	bool isPlayerHit_;

	// コントローラー
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	SelectEvent currentEvnectNo_;
	SelectEvent prevEventNo_;

	yunity::WorldTransform tvWorldTransform_;

	// TV出現演出用
	Easing grow_;
	Easing shrink_;

	// カメラワーク用
	float moveCameraTimer_;
	Vector3 oldCameraPos_;
	const Vector3 endCamerPos = { 0.0f, 20.0f, -60.0f };
	const float fixedEndCameraPosZ = 12.0f;
};

