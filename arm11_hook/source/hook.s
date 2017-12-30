.section .text.start
.align 4
.arm

.global arm11Hook
arm11Hook:
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
	cps #0x13			@ switch to supervisor mode
	cpsid aif
	push {r0-r5, lr}

	ldr r0, =0xFFFD6000
	mrc p15, 0, r1, c0, c0, 5 @ get core id
	and r1, #0x3
	lsl r1, #2 @ multiply r1 by 4

	mov r3, #0xf
	ldr r2, [r0, #0x204] @ LCD_TOP_FILL_REG
	orr r2, r3, lsl r1
	orr r2, #(1 << 24)
	str r2, [r0, #0x204]

	ldr r1, =0x10000FF
	ldr r2, [r0, #0x204] @ all cores visited ?
	cmp r2, r1
	beq _waitLoop_prep
	pop {r0-r5, lr}
	cps #0x12	@ switch back to IRQ mode
	ldr pc, =0xfff68144

	_waitLoop_prep:
	ldr r1, =0xFFFD2000	@ PXI_SYNC
	mov r2, #0xCC
	strb r2, [r1, #1] @ send 0xCC to arm9

	_waitLoop:
		ldr r4, =0xfffeff0
		ldr r3, =0xf0008			@ send sched. interrupt
		str r3, [r4]
		ldrb r3, [r1]
		cmp r3, #0xDD
		bne _waitLoop

	mov r8, #0
	str r8, [r0]
	mov r0, r8
	mov r1, r0
	mov r2, #2
	mov r3, #0
	ldr pc, =0xFFF60000
