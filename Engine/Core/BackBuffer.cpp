#include "BackBuffer.h"

#include <cassert>

#include "DirectXCore.h"
#include "DescriptorHeap.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

void yunity::BackBuffer::Finalize()
{
	backBuffers_.clear();
}

void yunity::BackBuffer::Create(IDXGISwapChain4* swapChain)
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	result = swapChain->GetDesc(&swapChainDesc);
	assert(SUCCEEDED(result));

	backBufferCount_ = swapChainDesc.BufferCount;

	rtvDescriptorHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kRTV);

	const uint32_t desriptorSizeRTV = rtvDescriptorHeap_->GetDescriptorSize();
	// 裏表の２つ分について
	backBuffers_.resize(swapChainDesc.BufferCount);
	// ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvDescriptorHeap_->Alloc(2).GetCPUHandle();
	for (int i = 0; i < backBuffers_.size(); i++) {
		// スワップチェーンからバッファを取得
		result = swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i]));
		assert(SUCCEEDED(result));
		/*rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart()*/;
		handle.ptr += (desriptorSizeRTV * i);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		yunity::Device::GetInstance()->GetDevice()->CreateRenderTargetView(backBuffers_[i].Get(), &renderTargetViewDesc, handle);
	}

	rtvHandles_[0] = rtvDescriptorHeap_->GetHeapPointer()->GetCPUDescriptorHandleForHeapStart()/*rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart()*/;
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + yunity::Device::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

void yunity::BackBuffer::SetRenderTarget(ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain, ID3D12DescriptorHeap* dsvHeap_)
{
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	commandList->OMSetRenderTargets(1, &rtvHandles_[bbIndex], false, &dsvHandle);
}

void yunity::BackBuffer::ClearRenderTarget(ID3D12GraphicsCommandList* commandList, IDXGISwapChain4* swapChain)
{
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 全画面クリア        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f }; // 青っぽい色
	commandList->ClearRenderTargetView(rtvHandles_[bbIndex], clearColor, 0, nullptr);
}
