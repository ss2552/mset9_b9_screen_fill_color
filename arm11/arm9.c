#include <inttypes.h>
#include "utils.9.h"
#include "constants.h"
#include "pointers.9.h"
#include "IFile.9.h"
#include "lib.9.h"

void __attribute__ ((section (".text.a9"))) core_main(void)
{
	disableInterrupts();
	flushCaches();

	// Mutex, ensure only one ARM9 thread executes this function
	// just in case this is needed.
	uint32_t* single = (uint32_t*)0x23FFFE0C;
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
			((uint8_t*)0x1848F000)[i] = 0x00;
			((uint8_t*)0x184C7800)[i] = 0x00;
		}
	}

	/* Size is completely arbitrary */
	// Find and assign the fb addresses
	*((uint32_t*)0x23FFFE00) = cur[0];
	*((uint32_t*)0x23FFFE04) = cur[1];
	*((uint32_t*)0x23FFFE08) = cur[2];

	finalJump();
}
