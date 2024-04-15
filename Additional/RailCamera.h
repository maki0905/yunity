#pragma once
#include "WorldTransform.h"
#include "Camera.h"
#include <vector>

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera, const Vector3& position, const Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(/*ViewProjection* viewProjection*/);

	//const WorldTransform& GetWorldMatrix() { return transform_; }
	const Camera* GetCamera() { return camera_; }

	WorldTransform& GetWorldMatrix() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	Camera* camera_;
	// スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;

	uint32_t eyeIndex_;
	uint32_t targetIndex_;
	size_t rangeManagemant_;
	size_t targetPoint_;

};
