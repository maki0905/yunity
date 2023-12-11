//#include "App.h"
//
//#include <cassert>
//
//#include "Device.h"
//#include "CommandList.h"
//#include "CommandQueue.h"
//#include "SwapChain.h"
//#include "BackBuffer.h"
//#include "DepthBuffer.h"
//#include "WindowsAPI.h"
//#include "Shader.h"
//#include "T.h"
//#include "Shader.h"
//
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "dxguid.lib")
//
//App::App() {
//	windowsAPI_ = WindowsAPI::GetInstance();
//	windowsAPI_->CreateGameWindow();
//	device_ = new Device();
//	
//	commandList_ = new CommandList(device_->GetDevice());
//	commandList_->Create();
//	commandQueue_ = new CommandQueue(device_->GetDevice());
//	commandQueue_->Create();
//	swapChain_ = new SwapChain(commandQueue_->GetCommandQueue(), device_->GetDxgiFactory());
//	swapChain_->Create();
//	backBuffer_ = new BackBuffer(device_->GetDevice(), commandList_->GetCommandList(), swapChain_->GetSwapChain());
//	backBuffer_->Create();
//	depthBuffer_ = new DepthBuffer(device_->GetDevice(), commandList_->GetCommandList());
//	depthBuffer_->Create();
//
//	windowWidth_ = WindowsAPI::GetInstance()->kWindowWidth;
//	windowHeight_ = WindowsAPI::GetInstance()->kWindowHeight;
//
//	shader_ = nullptr;
//	shader_ = Shader::GetInstance();
//	shader_->Initialize();
//	t_ = new T();
//	t_->Initialize(device_->GetDevice());
//	t_->Create();
//
//}
//
//App::~App() {
//	// ゲームウィンドウの破棄
//	windowsAPI_->TerminateGameWindow();
//}
//
//void App::Run()
//{
//	while (true) {
//		if (windowsAPI_->ProcessMessage()) {
//			break;
//		}
//
//		PreDraw();
//		t_->PreDraw(commandList_->GetCommandList());
//		t_->PostDraw();
//		PostDraw();
//
//	}
//}
//
//void App::PreDraw()
//{
//	commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
//	backBuffer_->SetRenderTarget(swapChain_->GetSwapChain(), depthBuffer_->GetDescriptorHeap());
//	backBuffer_->ClearRenderTarget(swapChain_->GetSwapChain());
//	depthBuffer_->ClearDepthView();
//
//	commandList_->SetViewport(float(windowWidth_), float(windowHeight_));
//	commandList_->SetRect(windowWidth_, windowHeight_);
//}
//
//void App::PostDraw()
//{
//
//	HRESULT result = S_FALSE;
//
//	commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
//
//	commandList_->GetCommandList()->Close();
//
//	// コマンドリストの実行
//	ID3D12CommandList* cmdLists[] = { commandList_->GetCommandList() }; // コマンドリストの配列
//	commandQueue_->GetCommandQueue()->ExecuteCommandLists(1, cmdLists);
//
//	// バッファをフリップ
//	result = swapChain_->GetSwapChain()->Present(1, 0);
//#ifdef _DEBUG
//	if (FAILED(result)) {
//		Microsoft::WRL::ComPtr<ID3D12DeviceRemovedExtendedData> dred;
//
//		result = device_->GetDevice()->QueryInterface(IID_PPV_ARGS(&dred));
//		assert(SUCCEEDED(result));
//
//		// 自動パンくず取得
//		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT autoBreadcrumbsOutput{};
//		result = dred->GetAutoBreadcrumbsOutput(&autoBreadcrumbsOutput);
//		assert(SUCCEEDED(result));
//	}
//#endif
//
//	commandQueue_->WaitForCommandsToFinish();
//	commandList_->CommandClear();
//}
