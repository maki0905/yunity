#include "CommandList.h"

#include <cassert>

#include "BackBuffer.h"
#include "DepthBuffer.h"
#include "Device.h"

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi.lib")

void yunity::CommandList::Create()
{
	CreateAllocator();
	CreateList();
}

void yunity::CommandList::BarrierChange(IDXGISwapChain4* swapChain, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	HRESULT result = S_FALSE;

	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// リソースバリアを変更（表示状態→描画対象）
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バックバッファのリソースを取得
	ID3D12Resource* pResource = nullptr;
	result = swapChain->GetBuffer(bbIndex, IID_PPV_ARGS(&pResource));
	//result = swapChain->GetBuffer(bbIndex, IID_PPV_ARGS(&barrier.Transition.pResource));
	// バリア設定
	barrier.Transition.pResource = pResource;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	// リソースバリアの適用
	commandList_->ResourceBarrier(1, &barrier);
	pResource->Release();

}

void yunity::CommandList::BarrierChange(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	// リソースバリアを変更
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Transition.pResource = resource;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	commandList_->ResourceBarrier(1, &barrier);
}

void yunity::CommandList::CommandClear()
{
	commandAllocator_->Reset();
	commandList_->Reset(commandAllocator_.Get(), nullptr);

}

void yunity::CommandList::OMSetRenderTargets(const D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescHandleRTV, ID3D12DescriptorHeap* dsvHeap)
{
	if (dsvHeap) {
		// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
			D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
		// レンダーターゲットをセット
		commandList_->OMSetRenderTargets(1, cpuDescHandleRTV, false, &dsvHandle);
	}
	else {
		// レンダーターゲットをセット
		commandList_->OMSetRenderTargets(1, cpuDescHandleRTV, false, nullptr);
	}
}

void yunity::CommandList::OMSetRenderTargets(const D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescHandleRTV, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle)
{
	// レンダーターゲットをセット
	commandList_->OMSetRenderTargets(1, cpuDescHandleRTV, false, dsvHandle);
}

void yunity::CommandList::ClearRenderTargetView(const Vector4 clearColorValue, const D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleRTV)
{
	// 全画面クリア        Red   Green Blue  Alpha
	//float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f }; // 青っぽい色
	float clearColor[] = { clearColorValue.x, clearColorValue.y, clearColorValue.z, clearColorValue.w };
	commandList_->ClearRenderTargetView(cpuDescHandleRTV, clearColor, 0, nullptr);
}

void yunity::CommandList::ClearDepthStencilView(ID3D12DescriptorHeap* dsvHeap)
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}


void yunity::CommandList::RSSetViewports(float width, float height)
{
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(0.0f, 0.0f, width, height, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH);
	commandList_->RSSetViewports(1, &viewport);

}

void yunity::CommandList::RSSetScissorRects(UINT width, UINT height)
{
	D3D12_RECT rect = D3D12_RECT(0, 0, width, height);
	commandList_->RSSetScissorRects(1, &rect);

}

ID3D12CommandList* yunity::CommandList::GetCommandLists()
{
	ID3D12CommandList* cmdLists[] = { commandList_.Get()};
	return *cmdLists;
}

void yunity::CommandList::CreateAllocator()
{
	HRESULT result = S_FALSE;
	result = Device::GetInstance()->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));
}

void yunity::CommandList::CreateList()
{
	HRESULT result = S_FALSE;
	result = Device::GetInstance()->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));
}
