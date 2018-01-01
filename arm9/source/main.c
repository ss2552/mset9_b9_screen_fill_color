#include "types.h"
#include "PXI.h"
#include "arm11.h"
#include "petitfs/pff.h"

#define CFG11_SHAREDWRAM_32K_DATA(i)	(*(vu8 *)(0x10140000 + i))
#define CFG11_SHAREDWRAM_32K_CODE(i)	(*(vu8 *)(0x10140008 + i))
#define CFG11_DSP_CNT					(*(vu8 *)0x10141230)

struct fb {
     u8 *top_left;
     u8 *top_right;
     u8 *bottom;
} ;

static const struct fb fbs[2] =
{
    {
        .top_left  = (u8 *)0x18300000,
        .top_right = (u8 *)0x18300000,
        .bottom    = (u8 *)0x18346500,
    },
    {
        .top_left  = (u8 *)0x18400000,
        .top_right = (u8 *)0x18400000,
        .bottom    = (u8 *)0x18446500,
    },
};

static FATFS sdFs;

static bool mountFs(void)
{
    return pf_mount(&sdFs) == FR_OK;
}

static bool fileRead(void *dest, const char *path, u32 maxSize)
{
    FRESULT result = FR_OK;
    u32 ret = 0;

    if(pf_open(path) != FR_OK) return false;

    result = pf_read(dest, maxSize, (unsigned int *)&ret);

    return result == FR_OK && ret != 0;
}

static bool readPayload(void)
{
    return mountFs() && fileRead((void *)0x23F00000, "/arm9.bin", 0x100000);
}

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
    bool payloadRead;

    while(PXIReceiveWord() != 0x44836);
    PXISendWord(0x964536);
    while(PXIReceiveWord() != 0x44837);
    PXIReceiveWord(); // High FIRM titleID
    PXIReceiveWord(); // Low FIRM titleID
    resetDSPAndSharedWRAMConfig();

    payloadRead = readPayload();
    while(PXIReceiveWord() != 0x44846);


    *(vu32 *)0x1FFFFFF8 = 0;
    memcpy((void *)0x1FFFF400, arm11FirmlaunchStub, arm11FirmlaunchStubSize);
    if(payloadRead)
        *(vu32 *)0x1FFFFFFC = 0x1FFFF400;
    else
    {
        *(vu32 *)0x1FFFFFFC = 0x1FFFF404; // fill the screens with red
        while(true);
    }
}

void main(void)
{
    memcpy((void *)0x23FFFE00, fbs, 2 * sizeof(struct fb));
    memcpy((void *)0x1FFF4C80, arm11Hook, arm11HookSize); // pretty much the same PA for all system versions
    *(vu32 *)0x1FFF4018 = 0xEA00031E; // Point the ARM11 IRQ vector to our code
    doFirmlaunch();
}
