#include "CommandQueue.h"

#include <cassert>

#pragma comment(lib, "d3d12")

CommandQueue::CommandQueue(ID3D12Device* device)
{
	device_ = device;
}

void CommandQueue::Create()
{
	HRESULT result = S_FALSE;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{}; // 標準設定
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));

	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
}

void CommandQueue::WaitForCommandsToFinish()
{
	// コマンドリストの実行完了を待つ
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}
