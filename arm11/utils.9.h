#ifndef _UTILS_9_H
#define _UTILS_9_H

#include <inttypes.h>

void __attribute__ ((section (".text.a9.entry"))) core_start(void);
int __attribute__ ((section (".text.a9"))) svcSleepThread9(uint64_t nanoSecs);

void __attribute__ ((section (".text.a9"))) reconfigureMPURegions(void);
void __attribute__ ((section (".text.a9"))) flushCaches(void);
void __attribute__ ((section (".text.a9"))) disableInterrupts(void);
void __attribute__ ((section (".text.a9"))) finalJump(void);
#endif
