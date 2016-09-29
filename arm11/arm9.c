#include <inttypes.h>
#include "utils.9.h"
#include "constants.h"
#include "pointers.9.h"
#include "IFile.9.h"
#include "lib.9.h"

void __attribute__ ((section (".text.a9"))) pxi_sync(void)
{
	*(volatile uint8_t*)0x10008003 |= 0x20;
}

uint32_t __attribute__ ((section (".text.a9"))) pxi_recv(void)
{
	while(*(volatile uint16_t*)0x10008004 & 0x100);
	return *(volatile uint32_t*)0x1000800C;
}

void __attribute__ ((section (".text.a9"))) pxi_send(uint32_t val)
{
	while(*(volatile uint16_t*)0x10008004 & 2);
	*(volatile uint32_t*)0x1000800C = 8;
}

void __attribute__ ((section (".text.a9"))) core_main(void)
{
	disableInterrupts();
	flushCaches();

	// Mutex, ensure only one ARM9 thread executes this function
	// just in case this is needed.
	volatile uint32_t* single = (volatile uint32_t*)0x23FFFE0C;
	if(*single == 0xAABBCCDD)
		while(1)
			svcSleepThread9(0x1DCD6500ULL);
	*single = 0xAABBCCDD;

	uint32_t *cur = (uint32_t *)0x20000000;

	reconfigureMPURegions();

	do {
		/* "FUCKFUCK" */
		if (*cur++ == MAGIC_WORD && *cur++ == MAGIC_WORD)
			break;
	} while ((uintptr_t)cur < (uintptr_t)0x28000000);

	if ((uintptr_t)cur == (uintptr_t)0x28000000) {

		/* Not found, let's just hang ourselves. */
		while(1)
		for (int i = 0; i < 0x00038400; ++i) {
			((volatile uint8_t*)0x1848F000)[i] = 0x00;
			((volatile uint8_t*)0x184C7800)[i] = 0x00;
		}
	}

	/* Size is completely arbitrary */
	// Find and assign the fb addresses
	*(volatile uint32_t*)0x23FFFE00 = cur[0];
	*(volatile uint32_t*)0x23FFFE04 = cur[1];
	*(volatile uint32_t*)0x23FFFE08 = cur[2];

	*(volatile uint32_t*)0x1FF882C0 = 0xE3B0A001; // patch k11 svc access check: tst r10, r11, lsl lr -> movs r10, #1
	*(volatile uint32_t*)0x1FFA6960 = 0xE3E0027E; // patch arm11 entrypoint address from 0x1FFFFFFC to 0x1FFFFFF8

	flushCaches(); // I'm not sure whether AXIWRAM is cached in our setup or not...

	// Send a reply to the PXIPS9_VerifyRsaSha256 command
	pxi_send(8); // pxi:ps
	pxi_sync();
	pxi_send(0x30041);
	pxi_send(0);
	pxi_send(4);

	// Fake a firmlaunch
	while(pxi_recv() != 0x44836);
	*(volatile uint8_t*)0x10141230 = 2; // DSP stuff
	*(volatile uint8_t*)0x10141230 = 0;
	pxi_send(0x964536);
	while(pxi_recv() != 0x44837);
	pxi_recv(); pxi_recv(); // High/Low FIRM titleID
	while(pxi_recv() != 0x44846);

	finalJump();
}
