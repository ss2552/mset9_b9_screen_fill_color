#pragma once

#include "types.h"

#define memcpy __builtin_memcpy
//void _memcpy(void* dst, const void* src, unsigned int size);
#define memset __builtin_memset
//void _memset(void* addr, int val, unsigned int size);
