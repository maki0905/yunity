#include "CommandList.h"

#include <cassert>

#pragma comment(lib, "d3d12")

CommandList::CommandList(ID3D12Device* device)
{
	device_ = device;
}

void CommandList::Create()
{
	CreateAllocator();
	CreateList();

}

void CommandList::CommandClear()
{
	commandAllocator_->Reset();
	commandList_->Reset(commandAllocator_.Get(), nullptr);

}

void CommandList::SetViewport(float width, float height)
{
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(0.0f, 0.0f, width, height);
	commandList_->RSSetViewports(1, &viewport);

}

void CommandList::SetRect(UINT width, UINT height)
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
