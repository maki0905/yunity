#include "DirectXCore.h"

#include <cassert>
#include <thread>

#include "WindowsAPI.h"
#include "Device.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"
#include "DepthBuffer.h"
#include "BackBuffer.h"


#pragma comment(lib, "d3d12.lib")

DirectXCore* DirectXCore::GetInstance()
{
	static DirectXCore instance;

	return &instance;
}

void DirectXCore::Initialize()
{
	// FPS固定初期化
	InitializeFixFPS();

	windowWidth_ = WindowsAPI::GetInstance()->kWindowWidth;
	windowHeight_ = WindowsAPI::GetInstance()->kWindowHeight;

	// デバイス
	device_ = Device::GetInstance();
	device_->Initialize();

	// コマンド関連
	commandList_ = new CommandList(device_->GetDevice());
	commandList_->Create();
	commandQueue_ = new CommandQueue(device_->GetDevice());
	commandQueue_->Create();

	// スワップチェーン
	swapChain_ = new SwapChain(commandQueue_->GetCommandQueue(), device_->GetDxgiFactory());
	swapChain_->Create();

	// 各種デスクリプタヒープ
	for (int i = 0; i < static_cast<int>(HeapType::kCount); i++) {
		descriptorHeaps_[i] = std::make_unique<DescriptorHeap>();
	}

	descriptorHeaps_[static_cast<int>(HeapType::kRTV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 3, false);
	descriptorHeaps_[static_cast<int>(HeapType::kSRV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
	descriptorHeaps_[static_cast<int>(HeapType::kDSV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	// バッファ
	depthBuffer_ = new DepthBuffer(device_->GetDevice(), commandList_->GetCommandList());
	depthBuffer_->Create();

	backBuffer_ = new BackBuffer(device_->GetDevice(), commandList_->GetCommandList(), swapChain_->GetSwapChain());
	backBuffer_->Create();

	// Shader
	shaderCompiler_ = ShaderCompiler::GetInstance();
	shaderCompiler_->Initialize();

	

}

void DirectXCore::PreDraw()
{
	commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	backBuffer_->SetRenderTarget(swapChain_->GetSwapChain(), depthBuffer_->GetDescriptorHeap());
	backBuffer_->ClearRenderTarget(swapChain_->GetSwapChain());
	depthBuffer_->ClearDepthView();

	commandList_->SetViewport(float(windowWidth_), float(windowHeight_));
	commandList_->SetRect(windowWidth_, windowHeight_);
}

void DirectXCore::PostDraw()
{
	HRESULT result = S_FALSE;
	commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList_->GetCommandList()->Close();


	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { commandList_->GetCommandList() }; // コマンドリストの配列
	commandQueue_->GetCommandQueue()->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ
	result = swapChain_->GetSwapChain()->Present(1, 0);
#ifdef _DEBUG
	if (FAILED(result)) {
		Microsoft::WRL::ComPtr<ID3D12DeviceRemovedExtendedData> dred;

		result = device_->GetDevice()->QueryInterface(IID_PPV_ARGS(&dred));
		assert(SUCCEEDED(result));

		// 自動パンくず取得
		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT autoBreadcrumbsOutput{};
		result = dred->GetAutoBreadcrumbsOutput(&autoBreadcrumbsOutput);
		assert(SUCCEEDED(result));
	}
#endif

	commandQueue_->WaitForCommandsToFinish();
	// FPS固定
	UpdateFixFPS();
	commandList_->CommandClear();

}

void DirectXCore::InitializeFixFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCore::UpdateFixFPS()
{
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	if (elapsed < kMinTime) {
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	reference_ = std::chrono::steady_clock::now();
}
