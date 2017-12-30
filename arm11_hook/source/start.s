.section .text.start
.align 4
.arm

.global _start
_start:
	@ Inspired from GW v1.0/old yls8 code
	b _arm11Hook
	b _arm11Hook
	b _arm11Hook
	b _arm11Hook
	b _arm11Hook
	b _arm11Hook
	b _arm11Hook
	b _arm11Hook
_arm11Hook:
	clrex
	cpsid aif, #0x13 @ switch to supervisor mode

	push {r0-r5, lr}
	mrc p15, 0, r0, c0, c0, 5 @ get core id
	and r0, #0x3
	bl signalCoreHooked

	cmp r0, #0
	pop {r0-r5, lr}
	bne _allHooked

	cps #0x12	@ switch back to IRQ mode
	ldr pc, =0xFFF68144 @ 2.1 specific

	_allHooked:
	b syncWithArm9AndDoFirmlaunch
