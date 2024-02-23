#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class SwapChain
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="commandQueue">コマンドキュー</param>
	/// <param name="dxgiFactory">DXGIファクトリー</param>
	SwapChain(ID3D12CommandQueue* commandQueue, IDXGIFactory7* dxgiFactory);

	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	IDXGISwapChain4* GetSwapChain() {
		return swapChain_.Get();
	}

private:

	/// <summary>
	/// ファクトリー生成
	/// </summary>
	void CreateFactory();

	/// <summary>
	/// スワップチェーン生成
	/// </summary>
	void CreateSwapChain();


private:

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;

};

