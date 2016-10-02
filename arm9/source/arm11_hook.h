#pragma once

#include "types.h"

#define ARM11_CORE_CONTROL_VALUE(i)	*(vu32 *)(0x23ffff00 + 4*(i))
#define MAGIC_BARRIER_WORD			0x12345678

extern const __attribute__((aligned(4))) u8 arm11Hook[];
extern const u32 arm11HookSize;
