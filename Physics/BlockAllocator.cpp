#include "BlockAllocator.h"

#include <cassert>
#include <limits.h>
#include <string.h>
#include <stddef.h>

static const uint32_t chunkSize = 16 * 1024;
static const uint32_t maxBlockSize = 640;
static const uint32_t chunkArrayIncrement = 120;

// サポートされるオブジェクトのサイズ
static const int32_t blockSizes[blockSizeCount] = {
	16,	  // 0
	32,	  // 1
	64,	  // 2
	96,	  // 3
	128,  // 4
	160,  // 5
	192,  // 6
	224,  // 7
	256,  // 8
	320,  // 9
	384,  // 10
	448,  // 11
	512,  // 12
	640,  // 13
};

// 任意の割り当てサイズを適切なスロットにマップする
struct SizeMap {
	SizeMap() {
		int32_t j = 0;
		values[0] = 0;
		for (int32_t i = 1; i <= maxBlockSize; ++i) {
			assert(j < blockSizeCount);
			if (i <= blockSizes[j]) {
				values[i] = (uint8_t)j;
			}
			else {
				++j;
				values[i] = (uint8_t)j;
			}
		}
	}
	uint8_t values[maxBlockSize + 1];
};

static const SizeMap sizeMap;

struct Chunk {
	int32_t blockSize;
	Block* blocks;
};

struct Block {
	Block* next;
};

BlockAllocator::BlockAllocator()
{
	assert(blockSizeCount < UCHAR_MAX);

	m_chunkSpace = chunkArrayIncrement;
	m_chunkCount = 0;
	m_chunks = (Chunk*)myAlloc(m_chunkSpace * sizeof(Chunk));

	memset(m_chunks, 0, m_chunkSpace * sizeof(Chunk));
	memset(m_freeLists, 0, sizeof(m_freeLists));

}

BlockAllocator::~BlockAllocator()
{
	for (int32_t i = 0; i < m_chunkCount; ++i) {
		myFree(m_chunks[i].blocks);
	}
	myFree(m_chunks);
}

void* BlockAllocator::Allocate(uint32_t size)
{
	if (size == 0) {
		return nullptr;
	}

	assert(0 < size);

	if (size > maxBlockSize) {
		return myAlloc(size);
	}

	int32_t index = sizeMap.values[size];
	assert(0 <= index && index < blockSizeCount);

	if (m_freeLists[index]) {
		// フリーリストの先頭のブロックを取得
		Block* block = m_freeLists[index];

		// フリーリストの先頭を取得したブロックの次のブロックに更新。
		m_freeLists[index] = block->next;

		return block;
	}
	else {

		if (m_chunkCount == m_chunkSpace) {
			// 古いチャンクのポインタを保存
			Chunk* oldChunks = m_chunks;

			// chunkSpaceを拡張
			m_chunkSpace += chunkArrayIncrement;

			// 新しいサイズでメモリを確保
			m_chunks = (Chunk*)myAlloc(m_chunkSpace * sizeof(Chunk));

			// 古いチャンクのデータを新しいチャンクにコピー
			memcpy(m_chunks, oldChunks, m_chunkCount * sizeof(Chunk));

			// 新しい領域を初期化
			memset(m_chunks + m_chunkCount, 0, chunkArrayIncrement * sizeof(Chunk));

			// 古い領域のメモリを解放
			myFree(oldChunks);
		}

		// 新しいチャンクのポインタをを取得
		Chunk* chunk = m_chunks + m_chunkCount;

		// 新しいチャンク内にブロックのメモリ領域を確保
		chunk->blocks = (Block*)myAlloc(chunkSize);

		/// ブロックのリンク付け

		// 対応するブロックサイズを取得
		int32_t blockSize = blockSizes[index];

		// チャンクにブロックサイズを設定
		chunk->blockSize = blockSize;

		// チャンク内に含まれるブロック数を計算
		int32_t blockCount = chunkSize / blockSize;

		assert(blockCount * blockSize <= chunkSize);
		for (int32_t i = 0; i < blockCount - 1; ++i) {
			// 現在のインデックスに対応するブロックのポインタを取得
			Block* block = (Block*)((int8_t*)chunk->blocks + blockSize * i);

			// 次のインデックスに対応するブロックのポインタを取得
			Block* next = (Block*)((int8_t*)chunk->blocks + blockSize * (i + 1));

			// 減殺のブロックから次のブロックへのポインタをリンク付け
			block->next = next;
		}
		// 最後のブロックに対応するポインタを取得
		Block* last = (Block*)((int8_t*)chunk->blocks + blockSize * (blockCount - 1));

		last->next = nullptr;

		// フリーリストに新しいチャンクの最初のブロックを追加
		m_freeLists[index] = chunk->blocks->next;

		// チャンク数を増やす
		++m_chunkCount;

		return chunk->blocks;

	}
}

void BlockAllocator::Free(void* p, uint32_t size)
{
	if (size == 0) {
		return;
	}

	assert(0 < size);

	if (size > maxBlockSize) {
		myFree(p);
		return;
	}

	int32_t index = sizeMap.values[size];
	assert(0 <= index && index < blockSizeCount);

	// 解放するメモリのポインタをキャスト
	Block* block = (Block*)p;

	// 解放されたブロックをフリーリストの先頭に挿入
	block->next = m_freeLists[index];

	// フリーリストの先頭を更新
	m_freeLists[index] = block;

}

void BlockAllocator::Clear()
{
	for (int32_t i = 0; i < m_chunkCount; ++i) {
		myFree(m_chunks[i].blocks);
	}

	m_chunkCount = 0;
	memset(m_chunks, 0, m_chunkSpace * sizeof(Chunk));
	memset(m_freeLists, 0, sizeof(m_freeLists));
}
