#pragma once

#include <d3d12.h>
#include <wrl.h>

class CommandQueue
{
public:

	void Finalize();
	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	/// <summary>
	/// 待機
	/// </summary>
	void WaitForCommandsToFinish();

	ID3D12CommandQueue* GetCommandQueue() {
		return commandQueue_.Get();
	}
	

private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Fence>fence_;
	UINT64 fenceVal_ = 0;
};

