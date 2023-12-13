#include "ImGuiManager.h"

#ifdef _DEBUG
#include "DirectXCore.h"
#include "WindowsAPI.h"
#include "DescriptorHeap.h"
#include "Device.h"
#include "Externals/imgui/imgui_impl_dx12.h"
#include "Externals/imgui/imgui_impl_win32.h"
#endif // _DEBUG


ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize()
{
#ifdef _DEBUG
	HRESULT result;
	directXCore_ = DirectXCore::GetInstance();
	Device* device = Device::GetInstance();
	srvHeap_ = directXCore_->GetDescriptorHeap(DirectXCore::HeapType::kSRV);


	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(WindowsAPI::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		device->GetDevice(), static_cast<int>(directXCore_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_->GetHeapPointer(),
		srvHeap_->GetHeapPointer()->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetHeapPointer()->GetGPUDescriptorHandleForHeapStart()
		/*srvHeap_->GetCPUDescriptorHandleForHeapStart(),*/
		/*srvHeap_->GetGPUDescriptorHandleForHeapStart()*/);

	ImGuiIO& io = ImGui::GetIO();
	// 標準フォントを追加する
	io.Fonts->AddFontDefault();

#endif // _DEBUG


}

void ImGuiManager::Finalize()
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

void ImGuiManager::Begin()
{
#ifdef _DEBUG
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

#endif

}

void ImGuiManager::End()
{
#ifdef _DEBUG
	// 描画前準備
	ImGui::Render();
#endif
}

void ImGuiManager::Draw()
{
#ifdef _DEBUG
	ID3D12GraphicsCommandList* commandList = directXCore_->GetCommandList();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { /*srvHeap_.Get()*/ srvHeap_->GetHeapPointer()};
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif

}
