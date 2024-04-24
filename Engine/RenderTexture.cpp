#include "RenderTexture.h"

#include <cassert>

#include "WindowsAPI.h"
#include "Device.h"
#include "DirectXCore.h"
#include "DescriptorHeap.h"


void RenderTexture::Create()
{
	CreateResorce();
	CreateRTV();
	CreateSRV();
	commandList_ = DirectXCore::GetInstance()->GetCommandList();
}

void RenderTexture::OMSetREnderTargets(ID3D12DescriptorHeap* dsvHeap_)
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	commandList_->OMSetRenderTargets(1, &cpuDescHandleRTV_, false, &dsvHandle);
}

void RenderTexture::ClearRenderTargetView()
{
	float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
	commandList_->ClearRenderTargetView(cpuDescHandleRTV_, clearColor, 0, nullptr);
}

void RenderTexture::CreateResorce()
{
	renderTextureResource = CreateRenderTextureResource(WindowsAPI::kWindowWidth, WindowsAPI::kWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
}

void RenderTexture::CreateRTV()
{
	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	DescriptorHeap* rtvDescriptorHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kRTV);
	// ディスクリプタヒープのハンドルを取得
	cpuDescHandleRTV_ = rtvDescriptorHeap_->Alloc().GetCPUHandle();

	Device::GetInstance()->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &renderTargetViewDesc, cpuDescHandleRTV_);
}

void RenderTexture::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;

	DescriptorHeap* srvDescriptorHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = srvDescriptorHeap_->Alloc().GetCPUHandle();

	Device::GetInstance()->GetDevice()->CreateShaderResourceView(renderTextureResource.Get(), &renderTextureSrvDesc, handle);

}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderTexture::CreateRenderTextureResource(uint32_t width, uint32_t hight, DXGI_FORMAT format, const Vector4& clearColor)
{
	HRESULT result = S_FALSE;

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = width; // リソースのサイズ
	resourceDesc.Height = hight;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	resourceDesc.Format = format;
	
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	result = Device::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET, // これから描画することを前提としたTextureなのでRenderTargetとして使うことから始める
		&clearValue, // Clear最適値。ClearRenderTargetをこの色でClearするようにする。最適化されているので高速である。
		IID_PPV_ARGS(&resource));


	assert(SUCCEEDED(result));

	return resource;
}
