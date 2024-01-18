#pragma once

#include "Setting.h"
#include "MathFunction.h"

const int32_t stackSize = 100 * 1024;
const int32_t maxStackEntries = 32;

struct StackEntry {
	char* data;
	int32_t size;
	bool usedMalloc;
};

class StackAllocator {
public:
	StackAllocator();
	~StackAllocator();

	// メモリを確保
	void* Allocate(int32_t size);

	// メモリを解放
	void Free(void* p);

	// 最大を取得
	int32_t GetMaxAllocation() const;
private:
	char m_data[stackSize];
	int32_t m_index;

	int32_t m_allocation;
	int32_t m_maxAllocation;

	StackEntry m_entries[maxStackEntries];
	int32_t m_entryCount;


};