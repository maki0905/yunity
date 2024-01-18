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

	// ���������蓖�āBmaxBlockSize���傫���ꍇ��myAlloc���g�p
	void* Allocate(uint32_t size);

	// ����������BmaxBlockSize���傫���ꍇ��myFree���g�p
	void Free(void* p, uint32_t size);

	// ������������
	void Clear();

private:

	Chunk* m_chunks;
	uint32_t m_chunkCount;
	uint32_t m_chunkSpace;

	Block* m_freeLists[blockSizeCount];
};