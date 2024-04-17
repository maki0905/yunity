#include "CommandList.h"

#include <cassert>

#include "BackBuffer.h"
#include "DepthBuffer.h"

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi.lib")

CommandList::CommandList(ID3D12Device* device)
{
	device_ = device;
}

void CommandList::Create()
{
	CreateAllocator();
	CreateList();
}

void CommandList::BarrierChange(IDXGISwapChain4* swapChain, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	HRESULT result = S_FALSE;

	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// リソースバリアを変更（表示状態→描画対象）
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	result = swapChain->GetBuffer(bbIndex, IID_PPV_ARGS(&barrier.Transition.pResource));
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	commandList_->ResourceBarrier(1, &barrier);

}

void CommandList::CommandClear()
{
	commandAllocator_->Reset();
	commandList_->Reset(commandAllocator_.Get(), nullptr);

}



void CommandList::OMSetRenderTargets(ID3D12DescriptorHeap* dsvHeap_)
{
	if (dsvHeap_) {
		// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
			D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	}
	// レンダーターゲットをセット
	commandList_->OMSetRenderTargets(1, &cpuDescHandleRTV_, false, &dsvHandle);
}

void CommandList::OMSetRenderTargets(const D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescHandleRTV, ID3D12DescriptorHeap* dsvHeap_)
{
	if (dsvHeap_) {
		// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
			D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
		// レンダーターゲットをセット
		commandList_->OMSetRenderTargets(1, cpuDescHandleRTV, false, &dsvHandle);
	}
	else {
		// レンダーターゲットをセット
		commandList_->OMSetRenderTargets(1, cpuDescHandleRTV, false, nullptr);
	}
}

void CommandList::ClearRenderTargetView()
{
}

void CommandList::ClearDepthStencilView()
{
}


void CommandList::RSSetViewports(float width, float height)
{
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(0.0f, 0.0f, width, height, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH);
	commandList_->RSSetViewports(1, &viewport);

}

void CommandList::RSSetScissorRects(UINT width, UINT height)
{
	D3D12_RECT rect = D3D12_RECT(0, 0, width, height);
	commandList_->RSSetScissorRects(1, &rect);

}

void CommandList::CreateAllocator()
{
	HRESULT result = S_FALSE;
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));
}

void CommandList::CreateList()
{
	HRESULT result = S_FALSE;
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));
}
