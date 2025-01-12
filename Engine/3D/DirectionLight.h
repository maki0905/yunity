#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <memory>

#include "Matrix4x4.h"
#include "PrimitiveDrawer.h"
#include "Camera.h"

namespace yunity {
	class DirectionLight {

		// 定数バッファ用データ構造体
		struct ConstBufferDataViewProjection {
			Matrix4x4 view;       // ワールド → ビュー変換行列
			Matrix4x4 projection; // ビュー → プロジェクション変換行列
		};
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(const Vector3& eyePostion, const Vector3& targetPosition, const Vector3& upDirection, float viewWidth, float viewHight, float nearClip, float farClip);

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		ID3D12Resource* GetLightBuff() const { return lightVPBuff_.Get(); }

		void SetEyePosition(const Vector3& eyePosition) { eyePostion_ = eyePosition; }
		void SetTargetPosition(const Vector3& targetPosition) { targetPosition_ = targetPosition; }
		void SetUpDirection_(const Vector3& upDirection) { upDirection_ = upDirection; }
		void SetViewWidth(float viewWidth) { viewWidth_ = viewWidth; }
		void SetviewHight(float viewHight) { viewHight_ = viewHight; }
		void SetNearClip(float nearClip) { nearClip_ = nearClip; }
		void SetFarClip(float farClip) { farClip_ = farClip; }


	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> lightVPBuff_;
		ConstBufferDataViewProjection* lightVP_ = nullptr;

		std::unique_ptr<PrimitiveDrawer> box_;
		WorldTransform worldTransform_;

		Vector3 eyePostion_;
		Vector3 targetPosition_;
		Vector3 upDirection_;
		float viewWidth_;
		float viewHight_;
		float nearClip_;
		float farClip_;

	};
}