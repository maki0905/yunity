#include "SwapChain.h"

#include <cassert>

#include "WindowsAPI.h"
#include "Device.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


void yunity::SwapChain::Create(ID3D12CommandQueue* commandQueue)
{
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = WindowsAPI::kWindowWidth;
	swapChainDesc.Height = WindowsAPI::kWindowHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;        // 色情報の書式を一般的なものに
	swapChainDesc.SampleDesc.Count = 1;                       // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;       // バックバッファとして使えるように
	swapChainDesc.BufferCount = 2;                            // バッファ数を２つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は速やかに破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; // ティアリングサポート
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
	result = Device::GetInstance()->GetDxgiFactory()->CreateSwapChainForHwnd(
		commandQueue, WindowsAPI::GetInstance()->GetHwnd(),
		&swapChainDesc, nullptr, nullptr, &swapChain1
	);
	assert(SUCCEEDED(result));

	// SwapChain4を得る
	swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));
	assert(SUCCEEDED(result));
}
