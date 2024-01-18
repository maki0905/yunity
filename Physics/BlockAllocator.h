#pragma once

#include <cstdint>

#include "Setting.h"

const uint32_t blockSizeCount = 14;

struct Block;
struct Chunk;

class BlockAllocator
{
public:
	BlockAllocator();
	~BlockAllocator();

	// メモリ割り当て。maxBlockSizeより大きい場合はmyAllocを使用
	void* Allocate(uint32_t size);

	// メモリ解放。maxBlockSizeより大きい場合はmyFreeを使用
	void Free(void* p, uint32_t size);

	// メモリ初期化
	void Clear();

private:

	Chunk* m_chunks;
	uint32_t m_chunkCount;
	uint32_t m_chunkSpace;

	Block* m_freeLists[blockSizeCount];
};