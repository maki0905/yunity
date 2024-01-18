#include "Setting.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void* myAlloc_Default(int32_t size)
{
    return malloc(size);
   
}

void myFree_Default(void* mem)
{
    free(mem);
}
