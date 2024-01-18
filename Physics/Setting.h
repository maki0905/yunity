#pragma once
#include <cstdint>

#include "Common.h"

void* myAlloc_Default(int32_t size);
void myFree_Default(void* mem);

inline void* myAlloc(int32_t size) {
	return myAlloc_Default(size);
}

inline void myFree(void* mem) {
	myFree_Default(mem);
}