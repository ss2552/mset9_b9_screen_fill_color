#include "memory.h"

void _memcpy(void* dst, const void* src, unsigned int size)
{
	char *destc = (char *) dst;
	const char *srcc = (const char *) src;
	for(uint32_t i = 0; i < size; i++)
	{
		destc[i] = srcc[i];
	}
}

void _memset(void* addr, int val, unsigned int size)
{
	char* caddr = (char*) addr;
	while(size--)
		*(caddr++) = val;
}
