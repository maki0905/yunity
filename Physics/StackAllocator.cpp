#include "StackAllocator.h"

#include <cassert>

StackAllocator::StackAllocator()
{
	m_index = 0;
	m_allocation = 0;
	m_maxAllocation = 0;
	m_entryCount = 0;
}

StackAllocator::~StackAllocator()
{
	assert(m_index == 0);
	assert(m_entryCount == 0);
}

void* StackAllocator::Allocate(int32_t size)
{
	assert(m_entryCount < maxStackEntries);

	// エントリーを取得
	StackEntry* entry = m_entries + m_entryCount;
	// サイズを設定
	entry->size = size;

	if (m_index + size > stackSize) {
		// メモリを確保
		entry->data = (char*)myAlloc(size);
		entry->usedMalloc = true;
	}
	else {
		// スタックメモリからメモリを確保
		entry->data = m_data + m_index;
		entry->usedMalloc = false;
		m_index += size;
	}

	// 現在のアロケーションサイズを更新します
	m_allocation += size;
	// 最大アロケーションサイズを更新します
	m_maxAllocation = Max(m_maxAllocation, m_allocation);
	++m_entryCount;

	return entry->data;
}

void StackAllocator::Free(void* p)
{
	assert(m_entryCount > 0);
	// 最後のエントリーを取得
	StackEntry* entry = m_entries + m_entryCount - 1;
	assert(p == entry->data);
	if (entry->usedMalloc) {
		// メモリの解放
		myFree(p);
	}
	else {
		// インデックスをエントリーのサイズだけ戻す
		m_index -= entry->size;
	}
	// 現在のアロケーションサイズからエントリーのサイズを減算
	m_allocation -= entry->size;
	--m_entryCount;
	p = nullptr;
}

int32_t StackAllocator::GetMaxAllocation() const
{
	return m_maxAllocation;
}
