.section .text.a9.entry
.align 4
.arm

.global core_start
.type	core_start, %function
core_start:
	nop
	bl core_main
	bx lr

.section .text.a9
.align 4
.arm

.macro SUPERVISOR label
	@ Jump to a label in supervisor mode

	mrs r0, cpsr
	tst r0, #0xf	@ Check whether we're in user-mode or not
	bne \label
	adr r0, \label
	svc 0x7b
	bx lr
.endm

.global svcSleepThread9
.type	svcSleepThread9, %function
svcSleepThread9:
	svc 0x0a
	bx lr

.global reconfigureMPURegions
.type	reconfigureMPURegions, %function
reconfigureMPURegions:
	SUPERVISOR _reconfigureMPURegions

_reconfigureMPURegions:
	ldr r0, =0x10000037
	mcr p15, 0, r0, c6, c3, 0	@ Region 3, 0x10000000-0x20000000

	mov r0, #0
	mcr p15, 0, r0, c6, c4, 0	@ Region 4, disable

	mcr p15, 0, r0, c6, c6, 0	@ Region 6, disable
	mcr p15, 0, r0, c6, c7, 0	@ Region 7, disable

	mrc p15, 0, r0, c5, c0, 3
	bic r0, r0, #0xf00000		@ Region 5 protect
	orr r0, r0, #0x300000		@ Region 5 r/w access
	mcr p15, 0, r0, c5, c0, 3
	bx lr

.global flushCaches
.type	flushCaches, %function
flushCaches:
	SUPERVISOR _flushCaches

_flushCaches:
	@ Clean and flush data cache
	mov r1, #0							@ segment counter
	outer_loop:
		mov r0, #0						@ line counter

		inner_loop:
			orr r2, r1, r0					@ generate segment and line address
			mcr p15, 0, r2, c7, c14, 2		@ clean and flush the line
			add r0, #0x20					@ increment to next line
			cmp r0, #0x400
			bne inner_loop

		add r1, #0x40000000
		cmp r1, #0
	bne outer_loop

	mcr p15, 0, r1, c7, c10, 4			@ drain write buffer

	@ Flush instruction cache
	mcr p15, 0, r1, c7, c5, 0
	bx lr

.global disableInterrupts
.type	disableInterrupts, %function
disableInterrupts:
	SUPERVISOR _disableInterrupts

_disableInterrupts:
	mrs r0, cpsr
	orr r0, #0x1c0	@ Disable IRQ/FIQ/Imprecise aborts
	msr cpsr_cx, r0
	bx lr

.global finalJump
.type	finalJump, %function
finalJump:
	SUPERVISOR _finalJump

_finalJump:
	bl disableInterrupts
	bl flushCaches

	@ Disable MPU
	ldr r0, =0x42078	@ alt vector select, enable itcm
	mcr p15, 0, r0, c1, c0, 0

	ldr r0, =#0x23f00000
	bx r0
