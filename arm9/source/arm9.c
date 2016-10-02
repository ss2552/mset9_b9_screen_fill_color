#include "types.h"
#include "utils.h"
#include "arm11_hook.h"
#include "memory.h"

static inline void pxi_sync(void)
{
	*(vu8 *)0x10008003 |= 0x20;
}

static inline u32 pxi_recv(void)
{
	while(*(vu16 *)0x10008004 & 0x100);
	return *(vu32 *)0x1000800C;
}

static inline void pxi_send(u32 val)
{
	while(*(vu16 *)0x10008004 & 2);
	*(vu32 *)0x10008008 = val;
}

static void resetDSPAndSharedWRAMConfig(void)
{
	*(vu8 *)0x10141230 = 2; // PDN_DSP_CNT
	for(volatile int i = 0; i < 10; i++);

	*(vu8 *)0x10141230 = 3;
	for(volatile int i = 0; i < 10; i++);

	for(int i = 0; i < 8; i++)
		*(vu8 *)(0x10140000 + i) = i << 2; // PDN_SHAREDWRAM_32K_DATA(i): disabled, master = arm9

	for(int i = 0; i < 8; i++)
		*(vu8 *)(0x10140008 + i) = i << 2; // PDN_SHAREDWRAM_32K_CODE(i): disabled, master = arm9

	for(int i = 0; i < 8; i++)
		*(vu8 *)(0x10140000 + i) = 0x80 | (i << 2); // PDN_SHAREDWRAM_32K_DATA(i): enabled, master = arm9

	for(int i = 0; i < 8; i++)
		*(vu8 *)(0x10140008 + i) = 0x80 | (i << 2); // PDN_SHAREDWRAM_32K_CODE(i): enabled, master = arm9
}

void main(void)
{
	*(vu32 *)0x23FFFE00 = fb[0];
	*(vu32 *)0x23FFFE04 = fb[1];
	*(vu32 *)0x23FFFE08 = fb[2];

	_memcpy((void *)0x1FFF4B40, arm11Hook, arm11HookSize);
	*(vu32 *)0x1FFF4018 = 0xEA0002CE; // Point the ARM11 IRQ vector to our code
	while(ARM11_CORE_CONTROL_VALUE(0) != MAGIC_BARRIER_WORD || ARM11_CORE_CONTROL_VALUE(1) != MAGIC_BARRIER_WORD);
	//*(vu32*)0x1FFA6960 = 0xE3E0027E; // patch arm11 entrypoint address from 0x1FFFFFFC to 0x1FFFFFF8

	// Fake a firmlaunch
	resetDSPAndSharedWRAMConfig();

	ARM11_CORE_CONTROL_VALUE(0) = 0xFFF60000; // firmlaunch function (located at the start of kernel code/AXIWRAM on 2.1)
	ARM11_CORE_CONTROL_VALUE(1) = 0xFFF60000;

	while(pxi_recv() != 0x44836);
	pxi_send(0x964536);
	while(pxi_recv() != 0x44837);
	pxi_recv(); pxi_recv(); // High/Low FIRM titleID
	while(pxi_recv() != 0x44846);

	finalJump();
}
