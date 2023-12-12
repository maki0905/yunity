#include "DirectXCore.h"
#include <cassert>

#include "DescriptorHeap.h"
#include "DescriptorHandle.h"

#pragma comment(lib, "d3d12.lib")

DirectXCore* DirectXCore::GetInstance()
{
	static DirectXCore instance;

	return &instance;
}

void DirectXCore::Initialize()
{


}

void DirectXCore::PreDraw()
{
}

void DirectXCore::PostDraw()
{
}
