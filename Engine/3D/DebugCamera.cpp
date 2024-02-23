﻿#include "DebugCamera.h"
#include "MathFunction.h"
#include "Transform.h"

DebugCamera::DebugCamera()
{
}

void DebugCamera::Update(Camera* camera)
{
	int32_t wheel = Input::GetInstance()->GetWheel();
	Vector2 mouseMove = Input::GetInstance()->GetMouseMove();
	Transform transform = { .scale{1.0f, 1.0f, 1.0f}, .rotate{camera->GetRotate()}, .translate{camera->GetTranslate()} };
	if (Input::GetInstance()->PushMouse(1)) {
		float rot = static_cast<float>(std::numbers::pi_v<float> / 180.0f);
		transform.rotate.x += rot * mouseMove.y * 0.1f;
		transform.rotate.y += rot * mouseMove.x * 0.1f;

		camera->SetRotate(transform.rotate);
		
		/*camera->rotation_.x += rot * mouseMove.y * 0.1f;
		camera->rotation_.y += rot * mouseMove.x * 0.1f;*/
	}
	else if (Input::GetInstance()->PushMouse(2)) {
		Matrix4x4 rotMat = MakeRotateXYZMatrix(transform.rotate);
		Vector3 cameraX = Multiply((static_cast<float>(-mouseMove.x) * 0.01f), GetXAxis(rotMat));
		Vector3 cameraY = Multiply((static_cast<float>(mouseMove.y) * 0.01f), GetYAxis(rotMat));
		Vector3 cameraXY = Add(cameraX, cameraY);
		transform.translate = Add(transform.translate, cameraXY);
		camera->SetTranslate(transform.translate);

		/*Matrix4x4 rotMat = MakeRotateXYZMatrix(camera->rotation_);
		Vector3 cameraX = Multiply((static_cast<float>(-mouseMove.x) * 0.01f), GetXAxis(rotMat));
		Vector3 cameraY = Multiply((static_cast<float>(mouseMove.y) * 0.01f), GetYAxis(rotMat));
		Vector3 cameraXY = Add(cameraX, cameraY);
		camera->translation_ = Add(camera->translation_, cameraXY);*/
	}
	else if (wheel != 0) {
		Matrix4x4 rotMat = MakeRotateXYZMatrix(transform.rotate);
		Vector3 cameraZ =  Multiply((static_cast<float>(wheel / 120.0f) * 0.5f), GetZAxis(rotMat));
		transform.translate = Add(transform.translate, cameraZ);
		camera->SetTranslate(transform.translate);

		/*Matrix4x4 rotMat = MakeRotateXYZMatrix(camera->rotation_);
		Vector3 cameraZ =  Multiply((static_cast<float>(wheel / 120.0f) * 0.5f), GetZAxis(rotMat));
		camera->translation_ = Add(camera->translation_, cameraZ);*/
	}
	camera->Update();
}
