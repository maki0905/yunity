#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class Device
{
public:
	
	Device();


	~Device();


	ID3D12Device* GetDevice() { return device_.Get(); }

	IDXGIFactory7* GetDxgiFactory() { return dxgiFactory_.Get(); }

private:
	void InitializeDXGIDevice();

private:
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;


};

