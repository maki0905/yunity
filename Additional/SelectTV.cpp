#include "SelectTV.h"

#include "Tradition.h"
#include "EngineTimeStep.h"
#include "MathFunction.h"
#include "RenderTexture.h"
#include "CommonData.h"

void SelectTV::Initialize()
{
	TVConstant tvConstant;
	tvWorldTransform_.Initialize();
	tvWorldTransform_.translation_ = worldTransform_.translation_;
	tvWorldTransform_.scale_ = { 0.0f, 0.0f, 0.0f };
	currentEvnectNo_ = SelectEvent::COUNT;
	isActiveTV_ = false;
	isPlayerHit_ = false;
	grow_ = { false, 0.0f };
	shrink_ = { false, 0.0f };
	moveCameraTimer_ = 0.0f;
	
}

void SelectTV::Update()
{
	prePad_ = pad_;

	switch (currentEvnectNo_)
	{
	case SelectTV::SelectEvent::GROW:
		Grow();
		break;
	case SelectTV::SelectEvent::SHRINK:
		Shrink();
		break;
	case SelectTV::SelectEvent::ZoomIn:
		ZoomIn();
		break;
	case SelectTV::SelectEvent::ZoomOut:
		ZoomOut();
		break;
	default:
		break;
	}

	if (isPlayerHit_) {
		if (yunity::Input::GetInstance()->IsControllerConnected()) {
			if (yunity::Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					if (!Tradition::GetInstance()->GetIsActive()) {
						oldCameraPos_ = camera_->GetTranslate();
						moveCameraTimer_ = 0.0f;
						Tradition::GetInstance()->Initialize();
						Tradition::GetInstance()->Start();
						float lenght = Length(Subtract(Vector3(player_->GetMatWorldTranslation().x, 0.0f, 0.0f), Vector3(tvWorldTransform_.GetMatWorldTranslation().x, 0.0f, 0.0f)));
						if (lenght == 0.0f) {
							camera_->SetTarget(nullptr);
						}
						currentEvnectNo_ = SelectEvent::ZoomOut;
						CommonData::GetInstance()->flagState_ = FlagState::kCount;

						oldPlayerPosition_ = player_->GetMatWorldTranslation();
						movePlayerTime_ = 0.0f;

					}

				}
			}
		}
	}
	else {
		if ((currentEvnectNo_ == SelectEvent::GROW && !isPlayerHit_) || (currentEvnectNo_ == SelectEvent::COUNT && !isPlayerHit_)) {
			shrink_ = { true, 0.0f, tvWorldTransform_.scale_ };
			currentEvnectNo_ = SelectEvent::SHRINK;
		}
	}

	isPlayerHit_ = false;
	tvWorldTransform_.UpdateMatrix();
}

void SelectTV::Draw()
{
	if (isActiveTV_) {
		model_->Draw(tvWorldTransform_);
	}
}

void SelectTV::OnTriggerEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isPlayerHit_ = true;
		player_ = GetHitBody();
		if (currentEvnectNo_ == SelectEvent::COUNT || currentEvnectNo_ == SelectEvent::SHRINK) {
			currentEvnectNo_ = SelectEvent::GROW;
			grow_ = { true, 0.0f, tvWorldTransform_.scale_ };
		}
	}
}

void SelectTV::Grow()
{
	if (grow_.t < 1.0f) {
		isActiveTV_ = true;
		grow_.t += yunity::fixedTimeStep_;
		tvWorldTransform_.scale_ = Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, std::clamp(grow_.t, 0.0f, 1.0f));
	}
}

void SelectTV::Shrink()
{
	if (shrink_.t < 1.0f) {
		shrink_.t += yunity::fixedTimeStep_;
		if (shrink_.t > 1.0f) {
			isActiveTV_ = false;
		}
		tvWorldTransform_.scale_ = Lerp(shrink_.scale, { 0.0f, 0.0f, 0.0f }, std::clamp(shrink_.t, 0.0f, 1.0f));
	}
}

void SelectTV::ZoomIn()
{
	moveCameraTimer_ += yunity::fixedTimeStep_;
	moveCameraTimer_ = std::clamp(moveCameraTimer_, 0.0f, 1.0f);
	camera_->SetTranslate(Lerp(oldCameraPos_, { oldCameraPos_.x, oldCameraPos_.y, fixedEndCameraPosZ }, moveCameraTimer_));
	Tradition::GetInstance()->Update();
}

void SelectTV::ZoomOut()
{
	float lenght = Length(Subtract(Vector3(player_->GetMatWorldTranslation().x, 0.0f, 0.0f), Vector3(tvWorldTransform_.GetMatWorldTranslation().x, 0.0f, 0.0f)));
	if (lenght != 0.0f) {
		movePlayerTime_ += yunity::fixedTimeStep_;
		movePlayerTime_ = std::clamp(movePlayerTime_, 0.0f, 1.0f);
		player_->SetTranslation(Lerp({oldPlayerPosition_.x, player_->GetMatWorldTranslation().y, 0.0f}, {tvWorldTransform_.GetMatWorldTranslation().x, player_->GetMatWorldTranslation().y, 0.0f}, movePlayerTime_));
	}
	else {
		if (camera_->GetTarget() != nullptr) {
			camera_->SetTarget(nullptr);
			oldCameraPos_ = camera_->GetTranslate();
		}
		moveCameraTimer_ += yunity::fixedTimeStep_;
		moveCameraTimer_ = std::clamp(moveCameraTimer_, 0.0f, 1.0f);
		camera_->SetTranslate(Lerp(oldCameraPos_, { oldCameraPos_.x, endCamerPos.y, endCamerPos.z }, moveCameraTimer_));
		if (moveCameraTimer_ >= 1.0f) {
			moveCameraTimer_ = 0.0f;
			oldCameraPos_ = camera_->GetTranslate();
			yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, true);
			currentEvnectNo_ = SelectEvent::ZoomIn;
		}
	}
}



