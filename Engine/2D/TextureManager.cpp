#include "TextureManager.h"
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/d3dx12.h"
#include <cassert>
#include <vector>

#include "DirectXCore.h"
#include "Device.h"

using namespace DirectX;

uint32_t yunity::TextureManager::Load(const std::string& fileName) {
	return TextureManager::GetInstance()->LoadInternal(fileName);
}

yunity::TextureManager* yunity::TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

void yunity::TextureManager::Initialize(std::string directoryPath) {
	directoryPath_ = directoryPath;

	intermediateResources_.clear();

	// デスクリプタサイズを取得
	sDescriptorHandleIncrementSize_ =
		Device::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 全テクスチャリセット
	ResetAll();
}

void yunity::TextureManager::ResetAll() {

	srvHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV);

	indexNextDescriptorHeap_ = 0;

	// 全テクスチャを初期化
	for (size_t i = 0; i < kNumDescriptors; i++) {
		textures_[i].resource.Reset();
		textures_[i].cpuDescHandleSRV.ptr = 0;
		textures_[i].gpuDescHandleSRV.ptr = 0;
		textures_[i].name.clear();
	}
}

const D3D12_RESOURCE_DESC yunity::TextureManager::GetResoureDesc(uint32_t textureHandle) {

	assert(textureHandle < textures_.size());
	Texture& texture = textures_.at(textureHandle);
	return texture.resource->GetDesc();
}

void yunity::TextureManager::SetGraphicsRootDescriptorTable(
	ID3D12GraphicsCommandList* commandList, UINT rootParamIndex,
	uint32_t textureHandle) { // デスクリプタヒープの配列
	assert(textureHandle < textures_.size());

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { /*srvHeap_.Get()*/ srvHeap_->GetHeapPointer() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// シェーダリソースビューをセット
	commandList->SetGraphicsRootDescriptorTable(
		rootParamIndex, textures_[textureHandle].gpuDescHandleSRV);
}

Microsoft::WRL::ComPtr<ID3D12Resource> yunity::TextureManager::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
{
	HRESULT hr;
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // uploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ。　今回はVector4を3頂点文
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> result = nullptr;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&result));
	assert(SUCCEEDED(hr));
	return result;
}

uint32_t yunity::TextureManager::LoadInternal(const std::string& fileName) {

	assert(indexNextDescriptorHeap_ < kNumDescriptors);
	uint32_t handle = indexNextDescriptorHeap_;
	std::string None = "";
	// 読み込み済みテクスチャを検索
	auto it = std::find_if(textures_.begin(), textures_.end(), [&](const auto& texture) {
		return texture.name == fileName;
		});
	if (it != textures_.end()) {
		// 読み込み済みテクスチャの要素番号を取得
		if (None == fileName) {
			handle = 0;
		}
		else {
			handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
		}
		return handle;
	}

	// 書き込むテクスチャの参照
	Texture& texture = textures_.at(handle);
	texture.name = fileName;

	// ディレクトリパスとファイル名を連結してフルパスを得る
	bool currentRelative = false;
	if (2 < fileName.size()) {
		currentRelative = (fileName[0] == '.') && (fileName[1] == '/');
	}
	std::string fullPath = currentRelative ? fileName : directoryPath_ + fileName;

	// ユニコード文字列に変換
	wchar_t wfilePath[256];
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath, _countof(wfilePath));

	std::string filePathW = fileName;
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	if (filePathW.ends_with(".dds")) { // .ddsで終わっていたらddsとみなす。
		// DDSテクスチャのロード
		//result = LoadFromDDSFile(wfilePath, DDS_FLAGS_NONE, nullptr, scratchImg);
		result = LoadFromDDSFile(wfilePath, DDS_FLAGS_NONE, &metadata, scratchImg);
	}
	else {
		// WICテクスチャのロード
		result = LoadFromWICFile(wfilePath, WIC_FLAGS_FORCE_SRGB, &metadata, scratchImg);
	}

	assert(SUCCEEDED(result));

	ScratchImage mipChain{};

	if (IsCompressed(scratchImg.GetMetadata().format)) { // 圧縮フォーマットかどうかを調べる
		mipChain = std::move(scratchImg); // 圧縮フォーマットならそのまま使うのでmoveする
	}
	else {
		// ミップマップ生成
		result = GenerateMipMaps(
			scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
			TEX_FILTER_DEFAULT, 0, mipChain);
	}
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	D3D12_RESOURCE_DESC texresDesc{};
	texresDesc.Format = metadata.format;
	texresDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);
	texresDesc.Width = UINT(metadata.width);
	texresDesc.Height = UINT(metadata.height);
	texresDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	texresDesc.MipLevels = UINT16(metadata.mipLevels);
	texresDesc.SampleDesc.Count = 1;
	texresDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	texresDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

	// テクスチャ用バッファの生成
	result = Device::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし。
		&texresDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST, // テクスチャ用指定
		nullptr,  // Clear最適値。使わないでnullptr
		IID_PPV_ARGS(&texture.resource)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送

	DescriptorHandle srvHandle = srvHeap_->Alloc();

	// シェーダリソースビュー作成
	texture.cpuDescHandleSRV = srvHandle.GetCPUHandle();
	texture.gpuDescHandleSRV = srvHandle.GetGPUHandle();
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texture.resource->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	if (metadata.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0; // 
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
		srvDesc.Texture2D.MipLevels = (UINT)metadata.mipLevels;
	}

	Device::GetInstance()->GetDevice()->CreateShaderResourceView(
		texture.resource.Get(), //ビューと関連付けるバッファ
		&srvDesc,               //テクスチャ設定情報
		texture.cpuDescHandleSRV);

	indexNextDescriptorHeap_++;

	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(Device::GetInstance()->GetDevice(), scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.resource.Get(), 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(Device::GetInstance()->GetDevice(), intermediateSize);
	UpdateSubresources(DirectXCore::GetInstance()->GetCommandList(), texture.resource.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	// Tetureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENRIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.resource.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	DirectXCore::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	intermediateResources_.emplace_back(intermediateResource);

	return handle;
}


