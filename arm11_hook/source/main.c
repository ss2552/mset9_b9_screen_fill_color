#include "types.h"

typedef struct VersionInfo {
    void (*firmlaunch)(void);
    u32 lcdBase;
    u32 mpcoreBase;
    u32 mask;
} VersionInfo;

///////////////////////////////////////////////////////
#define firmlaunch (info->firmlaunch)
#define LCD_REGS_BASE           (info->lcdBase)
#define MPCORE_REGS_BASE        (info->mpcoreBase)
#define BITMASK                 (info->mask)
///////////////////////////////////////////////////////

#define LCD_TOP_FILL_REG        *(vu32 *)(LCD_REGS_BASE + 0x200 + 4)
#define LCD_BOTTOM_FILL_REG     *(vu32 *)(LCD_REGS_BASE + 0xA00 + 4)
#define LCD_FILL_ENABLE         (1u << 24)

#define MPCORE_GID_REGS_BASE    (MPCORE_REGS_BASE + 0x1000)
#define MPCORE_GID_SGI          (*(vu32 *)(MPCORE_GID_REGS_BASE + 0xF00))

bool signalCoreHooked(u32 coreId, VersionInfo *info)
{
    u32 mask = BITMASK;
    LCD_TOP_FILL_REG |= LCD_FILL_ENABLE | (0xF << (8 + 4 * coreId));
    return (LCD_TOP_FILL_REG & mask) == mask; // expect green if 2 cores are present
}

void syncWithArm9AndDoFirmlaunch(u32 coreId, VersionInfo *info)
{
    u32 mask = BITMASK;
    LCD_BOTTOM_FILL_REG |= LCD_FILL_ENABLE | (0xF << (8 + 4 * coreId));
    while((LCD_BOTTOM_FILL_REG & mask) != mask)
        MPCORE_GID_SGI = 0x1F0008; // send scheduler interrupt to all cores except us => redirect to our code

    LCD_TOP_FILL_REG &= ~LCD_FILL_ENABLE;
    LCD_BOTTOM_FILL_REG &= ~LCD_FILL_ENABLE;
    firmlaunch();
}
