#pragma once

#include <array>
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>

/// <summary>
/// キー入力
/// </summary>
class Input
{
public:
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等 )</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等 )</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// 全キー情報取得
	/// </summary>
	/// <returns>全キー情報</returns>
	const std::array<BYTE, 256>& GetAllKey() { return key_; }

private:
	/*Input() = default;
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;*/
private:
	Microsoft::WRL::ComPtr<IDirectInput8> dInput_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> devKeyboard_;
	std::array<BYTE, 256> key_;
	std::array<BYTE, 256> keyPre_;
};

