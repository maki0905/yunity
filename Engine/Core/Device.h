#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

/*
* @brief デバイスクラス
*/
class Device
{
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static Device* GetInstance();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// getter
	/// </summary>
	/// <returns></returns>
	
	ID3D12Device* GetDevice() { return device_.Get(); } 
	IDXGIFactory7* GetDxgiFactory() { return dxgiFactory_.Get(); }

private:
	Device() = default;
	~Device() = default;
	Device(const Device&) = delete;
	Device& operator=(const Device&) = delete;
private:
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
};

