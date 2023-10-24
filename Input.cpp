#include "Input.h"
#include <cassert>

#include "WinApp.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "XInput.lib")

Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize()
{
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成
	result = DirectInput8Create(
		WinApp::GetInstance()->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = dInput_->CreateDevice(GUID_SysKeyboard, &devKeyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = devKeyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result =
		devKeyboard_->SetCooperativeLevel(WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));


}

void Input::Update()
{
	devKeyboard_->Acquire(); // キーボード動作開始

	// 前回のキー入力を保存
	keyPre_ = key_;

	// キーの入力
	devKeyboard_->GetDeviceState((DWORD)size(key_), key_.data());

}

bool Input::PushKey(BYTE keyNumber)
{
	// 0でなければ押している
	if (key_[keyNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::PrePushKey(BYTE keyNumber) const
{
	if (!keyPre_[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 前が0で、今回が0でなければトリガー
	if (!keyPre_[keyNumber] && key_[keyNumber]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::IsKeyReleased(BYTE keyNumber) const
{
	// 前が0ではなくて、今回が0
	if (keyPre_[keyNumber] && !key_[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::ExitKey(BYTE keyNumber) const
{
	// 前回が0ではなくて、今回が0
	if (keyPre_[keyNumber] && !key_[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::PushMouse(int32_t keyNumber) const
{
	if (mouse_.rgbButtons[keyNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerMouse(int32_t keyNumber) const
{
	// 前が0で、今回が0でなければトリガー
	if (!mousePre_.rgbButtons[keyNumber] && mouse_.rgbButtons[keyNumber]) {
		return true;
	}
	// トリガーでない
	return false;
}

bool Input::ExitMouse(int32_t keyNumber) const
{
	// 前が0ではなくて、今回が0
	if (mousePre_.rgbButtons[keyNumber] && !mouse_.rgbButtons[keyNumber]) {
		return true;
	}
	return false;
}

int32_t Input::GetWheel() const
{
	return static_cast<int32_t>(mouse_.lZ);
}

Vector2 Input::GetMouseMove() const
{
	return { (float)mouse_.lX,(float)mouse_.lY };
}

bool Input::GetJoystickState(int32_t stickNo, DIJOYSTATE2& out) const
{
	if (stickNo >= 0 && stickNo < static_cast<int32_t>(devJoysticks_.size())) {
		out = devJoysticks_[stickNo].state_.directInput_;
		return true;
	}
	return false;
}

bool Input::GetJoystickStatePrevious(int32_t stickNo, DIJOYSTATE2& out) const
{
	if (stickNo >= 0 && stickNo < static_cast<int32_t>(devJoysticks_.size())) {
		out = devJoysticks_[stickNo].statePre_.directInput_;
		return true;
	}
	return false;
}

bool Input::GetJoystickState(int32_t stickNo, XINPUT_STATE& out) const
{
	if (stickNo >= 0 && stickNo < static_cast<int32_t>(devJoysticks_.size())) {
		if (devJoysticks_[stickNo].type_ == PadType::XInput) {
			out = devJoysticks_[stickNo].state_.xInput_;
			return true;
		}
		else {
			// ジョイスティックが接続されていない場合
			return false;
		}
	}
	return false;
}

bool Input::GetJoystickStatePrevious(int32_t stickNo, XINPUT_STATE& out) const
{
	if (stickNo >= 0 && stickNo < static_cast<int32_t>(devJoysticks_.size())) {
		if (devJoysticks_[stickNo].type_ == PadType::XInput) {
			out = devJoysticks_[stickNo].statePre_.xInput_;
			return true;
		}
	}
	return false;
}

bool Input::IsControllerConnected() const
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i) {
		XINPUT_STATE state;
		if (XInputGetState(i, &state) == ERROR_SUCCESS) {
			// コントローラーが接続されている
			return true;
		}
	}
	// どのコントローラーも接続されていない
	return false;
}
