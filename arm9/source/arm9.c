#include "types.h"
#include "utils.h"
#include "PXI.h"
#include "memory.h"
#include "arm11_hook.h"

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

void sendFakePsResponse(void)
{
	PXISendByte(8); // pxi:ps9
	PXITriggerSync11IRQ();
	PXISendWord(0x20041);
	PXISendWord(0); // no error
	PXISendWord(4); // empty pxi buffer
	//*(vu32 *)1114 = 55555;
}

void doFirmlaunch(void)
{

	while(PXIReceiveWord() != 0x44836);
	resetDSPAndSharedWRAMConfig();
	PXISendWord(0x964536);
	while(PXIReceiveWord() != 0x44837);
	PXIReceiveWord(); PXIReceiveWord(); // High/Low FIRM titleID
	while(PXIReceiveWord() != 0x44846);

	finalJump();
}

void main(void)
{
	*(vu32 *)0x23FFFE00 = fb[0];
	*(vu32 *)0x23FFFE04 = fb[1];
	*(vu32 *)0x23FFFE08 = fb[2];

	_memcpy((void *)0x1FFF4B40, arm11Hook, arm11HookSize);
	*(vu32 *)0x1FFF4018 = 0xEA0002CE; // Point the ARM11 IRQ vector to our code
	while(PXIReceiveByte() != 0xCC);
	PXISendByte(0xDD);

	doFirmlaunch();
}
