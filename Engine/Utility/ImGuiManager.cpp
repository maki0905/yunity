#include "ImGuiManager.h"

#ifdef _DEBUG
#include "DirectXCore.h"
#include "WindowsAPI.h"
#include "DescriptorHeap.h"
#include "Device.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#endif // _DEBUG


yunity::ImGuiManager* yunity::ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void yunity::ImGuiManager::Initialize()
{
#ifdef _DEBUG
	DescriptorHandle srvHandle = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV)->Alloc();
	IMGUI_CHECKVERSION();
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(WindowsAPI::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		Device::GetInstance()->GetDevice(),
		static_cast<int>(DirectXCore::GetInstance()->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 
		DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV)->GetHeapPointer(),
		srvHandle.GetCPUHandle(),
		srvHandle.GetGPUHandle()
		);

	ImGuiIO& io = ImGui::GetIO();
	// 標準フォントを追加する
	io.Fonts->AddFontDefault();

#endif // _DEBUG


}

void yunity::ImGuiManager::Finalize()
{
#ifdef _DEBUG
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デスクリプタヒープを解放
	//srvHeap_.Reset();
#endif

}

void yunity::ImGuiManager::Begin()
{
#ifdef _DEBUG
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

#endif

}

void yunity::ImGuiManager::End()
{
#ifdef _DEBUG
	// 描画前準備
	ImGui::Render();
#endif
}

void yunity::ImGuiManager::Draw()
{
#ifdef _DEBUG
	//ID3D12GraphicsCommandList* commandList = directXCore_->GetCommandList();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { /*srvHeap_.Get()*/ DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV)->GetHeapPointer()};
	DirectXCore::GetInstance()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCore::GetInstance()->GetCommandList());

#endif

}
