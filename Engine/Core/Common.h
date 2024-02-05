#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <cassert>

/// <summary>
///  定数バッファ生成
/// </summary>
/// <param name="sizeInBytes"></param>
/// <returns>サイズ</returns>
ID3D12Resource* CreateBufferResource(size_t sizeInBytes);