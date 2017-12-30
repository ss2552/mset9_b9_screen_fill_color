#include "types.h"
#include "utils.h"
#include "PXI.h"
#include "memory.h"
#include "arm11_hook.h"

#define CFG11_SHAREDWRAM_32K_DATA(i)	(*(vu8 *)(0x10140000 + i))
#define CFG11_SHAREDWRAM_32K_CODE(i)	(*(vu8 *)(0x10140008 + i))
#define CFG11_DSP_CNT					(*(vu8 *)0x10141230)

static void resetDSPAndSharedWRAMConfig(void)
{
	CFG11_DSP_CNT = 2; // PDN_DSP_CNT
	for(volatile int i = 0; i < 10; i++);

	CFG11_DSP_CNT = 3;
	for(volatile int i = 0; i < 10; i++);

	for(int i = 0; i < 8; i++)
		CFG11_SHAREDWRAM_32K_DATA(i) = i << 2; // disabled, master = arm9

	for(int i = 0; i < 8; i++)
		CFG11_SHAREDWRAM_32K_CODE(i) = i << 2; // disabled, master = arm9

	for(int i = 0; i < 8; i++)
		CFG11_SHAREDWRAM_32K_DATA(i) = 0x80 | (i << 2); // enabled, master = arm9

	for(int i = 0; i < 8; i++)
		CFG11_SHAREDWRAM_32K_CODE(i) = 0x80 | (i << 2); // enabled, master = arm9
}

static void doFirmlaunch(void)
{

	while(PXIReceiveWord() != 0x44836);
	PXISendWord(0x964536);
	while(PXIReceiveWord() != 0x44837);
	PXIReceiveWord(); // High FIRM titleID
	PXIReceiveWord(); // Low FIRM titleID
	resetDSPAndSharedWRAMConfig();
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
