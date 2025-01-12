#include "DirectionLight.h"

#include "Common.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

void yunity::DirectionLight::Initialize(const Vector3& eyePostion, const Vector3& targetPosition, const Vector3& upDirection, float viewWidth, float viewHight, float nearClip, float farClip)
{
	lightVPBuff_ = CreateBufferResource(sizeof(ConstBufferDataViewProjection));
	lightVP_ = nullptr;
	lightVPBuff_->Map(0, nullptr, (void**)&lightVP_);
	lightVP_->view = MakeMatrixLookAt(eyePostion, targetPosition, upDirection);
	lightVP_->projection = MakeOrthographicMatrix(viewWidth, viewHight, nearClip, farClip);
	eyePostion_ = eyePostion;
	targetPosition_ = targetPosition;
	upDirection_ = upDirection;
	viewWidth_ = viewWidth;
	viewHight_ = viewHight;
	nearClip_ = nearClip;
	farClip_ = farClip;

	box_ = std::make_unique<PrimitiveDrawer>();
	box_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
	box_->SetCamera(CameraManager::GetInstance()->GetCamera());
	box_->SetColor({ 0.0f, 1.0f, 1.0f, 1.0f });

	worldTransform_.Initialize();
	worldTransform_.translation_ = eyePostion;
	worldTransform_.scale_ = { viewWidth, viewHight, farClip - nearClip };
}

void yunity::DirectionLight::Update()
{
#ifdef _DEBUG
	ImGui::Begin("DirectionLight");
	ImGui::DragFloat3("eyePostion", &eyePostion_.x);
	ImGui::DragFloat3("targetPosition", &targetPosition_.x);
	ImGui::DragFloat3("upDirection", &upDirection_.x);
	ImGui::DragFloat("viewWidth", &viewWidth_);
	ImGui::DragFloat("viewHight", &viewHight_);
	ImGui::DragFloat("nearClip", &nearClip_);
	ImGui::DragFloat("farClip", &farClip_);
	ImGui::End();

	std::array<Vector3, 8> corners;
	corners[0] = { -1.0f,  1.0f,  0.0f};
	corners[1] = {  1.0f,  1.0f,  0.0f};
	corners[2] = {  1.0f, -1.0f,  0.0f};
	corners[3] = { -1.0f, -1.0f,  0.0f};
	corners[4] = { -1.0f,  1.0f,  1.0f};
	corners[5] = {  1.0f,  1.0f,  1.0f};
	corners[6] = {  1.0f, -1.0f,  1.0f};
	corners[7] = { -1.0f, -1.0f,  1.0f};

	Matrix4x4 vp = Multiply(MakeMatrixLookAt(eyePostion_, targetPosition_, upDirection_), MakeOrthographicMatrix(viewWidth_, viewHight_, nearClip_, farClip_));
	vp = Inverse(vp);
	std::array<Vector3, 8> worldConers;
	for (int i = 0; i < 8; i++) {
		worldConers[i] = TransformVector3(corners[i], vp);
	}

	Vector3 center = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < 8; i++) {
		center = Add(center, worldConers[i]);
	}
	//eyePostion_ = Multiply(1.0f / 8.0f,  center);
	viewWidth_ = Length(Subtract(worldConers[1], worldConers[0]));
	viewHight_ = Length(Subtract(worldConers[0], worldConers[3]));

#endif

	upDirection_ = Normalize(upDirection_);

	lightVP_->view = MakeMatrixLookAt(eyePostion_, targetPosition_, upDirection_);
	lightVP_->projection = MakeOrthographicMatrix(viewWidth_, viewHight_, nearClip_, farClip_);
	worldTransform_.translation_ = eyePostion_;
	worldTransform_.scale_ = { viewWidth_, viewHight_, farClip_ - nearClip_ };

	worldTransform_.UpdateMatrix();
}

void yunity::DirectionLight::Draw()
{
	box_->Draw(worldTransform_);
}