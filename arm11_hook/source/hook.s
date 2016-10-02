.section .text.start
.align 4
.arm

.global arm11Hook
arm11Hook:
	@ Shamelessly "adapted" from GW v1.0
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
	push {r0-r5, lr}

	mrc p15, 0, r0, c0, c0, 5
	tst r0, #3			@ get core ID

	ldr r0, =0xf3ffff00	@ PA = 0x23ffff00. We'll be putting control variables here
	addne r0, #4		@ control variable for the current core
	eor r2, r0, #4		@ control variable for the other core

	bl flushCaches

	ldr r5, =0x12345678 @ magic control value (to signal we're waiting / detect when to handle interrupt requests)
	str r0, [r0]		@ overwrite the control variable for the current core (barrier)

	bl flushCaches

	ldr r3, [r2]
	cmp r3, r2
	beq _waitLoop_prep
	cmp r3, r5
	beq _waitLoop_prep
	pop {r0-r5, lr}
	cps #0x12	@ switch back to IRQ mode
	ldr pc, =0xfff68144

	_waitLoop_prep:
	str r5, [r0]

	_waitLoop:
		ldr r4, =0xfffeff00			@ L2 cache controller test register (?)
		ldr r3, =0xf0008
		str r3, [r4]
		mcr p15, 0, r1, c7, c14, 0	@ flush caches
		mcr p15, 0, r1, c7, c10, 4
		ldr r3, [r0]
		cmp r3, r5
		movne pc, r3
		b _waitLoop

.global flushCaches
.type   flushCaches, %function
flushCaches:
	mov r1, #0
	mcr p15, 0, r1, c7, c14, 0	@ "Clean and Invalidate Entire Data Cache"
	mcr p15, 0, r1, c7, c10, 5	@ "Data Memory Barrier"
	mcr p15, 0, r1, c7, c5,  0	@ "Invalidate Entire Instruction Cache. Also flushes the branch target cache"
	mcr p15, 0, r1, c7, c10, 4	@ "Data Synchronization Barrier"
	bx lr
