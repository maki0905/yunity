#include "BackBuffer.h"

#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

BackBuffer::BackBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain)
{
	device_ = device;
	commandList_ = commandList;
	swapChain_ = swapChain;
}

void BackBuffer::Create()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	result = swapChain_->GetDesc(&swapChainDesc);
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	heapDesc.NumDescriptors = swapChainDesc.BufferCount;
	result = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvDescriptorHeap_));
	assert(SUCCEEDED(result));

	const uint32_t desriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// 裏表の２つ分について
	backBuffers_.resize(swapChainDesc.BufferCount);
	for (int i = 0; i < backBuffers_.size(); i++) {
		// スワップチェーンからバッファを取得
		result = swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i]));
		assert(SUCCEEDED(result));

		// ディスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += (desriptorSizeRTV * i);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &renderTargetViewDesc, handle);
	}

	rtvHandles_[0] = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

void BackBuffer::SetRenderTarget(IDXGISwapChain4* swapChain, ID3D12DescriptorHeap* dsvHeap_)
{
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	commandList_->OMSetRenderTargets(1, &rtvHandles_[bbIndex], false, &dsvHandle);

}

void BackBuffer::ClearRenderTarget(IDXGISwapChain4* swapChain)
{
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 全画面クリア        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f }; // 青っぽい色
	commandList_->ClearRenderTargetView(rtvHandles_[bbIndex], clearColor, 0, nullptr);
}
