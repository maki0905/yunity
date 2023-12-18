//#include "DebugCamera.h"
//
//DebugCamera::DebugCamera()
//{
//}
//
//void DebugCamera::Update(ViewProjection* viewProjection)
//{
//	int32_t wheel = Input::GetInstance()->GetWheel();
//	Vector2 mouseMove = Input::GetInstance()->GetMouseMove();
//	if (Input::GetInstance()->PushMouse(1)) {
//		float rot = static_cast<float>(pi / 180.0f);
//		viewProjection->rotation_.x += rot * mouseMove.y * 0.1f;
//		viewProjection->rotation_.y += rot * mouseMove.x * 0.1f;
//	}
//	else if (Input::GetInstance()->PushMouse(2)) {
//		Matrix4x4 rotMat = MakeRotateXYZMatrix(viewProjection->rotation_);
//		Vector3 cameraX = Multiply((static_cast<float>(-mouseMove.x) * 0.01f), GetXAxis(rotMat));
//		Vector3 cameraY = Multiply((static_cast<float>(mouseMove.y) * 0.01f), GetYAxis(rotMat));
//		Vector3 cameraXY = Add(cameraX, cameraY);
//		viewProjection->translation_ = Add(viewProjection->translation_, cameraXY);
//	}
//	else if (wheel != 0) {
//		Matrix4x4 rotMat = MakeRotateXYZMatrix(viewProjection->rotation_);
//		Vector3 cameraZ =  Multiply((static_cast<float>(wheel / 120.0f) * 0.5f), GetZAxis(rotMat));
//		viewProjection->translation_ = Add(viewProjection->translation_, cameraZ);
//	}
//	viewProjection->UpdateMatrix();
//}
