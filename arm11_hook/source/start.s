.section .text.start
.align 4
.arm

.global _start
_start:
    @ Inspired from GW v1.0/old yls8 code
    clrex
    cpsid aif, #0x13 @ switch to supervisor mode
    push {r0-r3, r12, lr}

    mrc p15, 0, r0, c0, c0, 5 @ get core id
    and r0, #0x3
    adr r1, _info
    bl signalCoreHooked

    cmp r0, #0
    pop {r0-r3, r12, lr}
    bne _allHooked

    cps #0x12	@ switch back to IRQ mode
    ldr pc, =0xFFFF0018

    _allHooked:
    mrc p15, 0, r0, c0, c0, 5 @ get core id
    and r0, #0x3
    adr r1, _info
    b syncWithArm9AndDoFirmlaunch

_info:
.word 0xAAAAAAAA, 0, 0, 0
