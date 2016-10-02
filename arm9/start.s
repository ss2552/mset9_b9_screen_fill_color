.section .text.start
.align 4
.arm

.global _start
_start:
	nop
	mov r0, pc
	svc 0x7b
	nop

	@ Disable interrupts
	mrs r0, cpsr
	orr r0, #0x1c0	@ Disable IRQ/FIQ/Imprecise aborts
	msr cpsr_cx, r0

	@ Disable caches / MPU
	mrc p15, 0, r0, c1, c0, 0  @ read control register
	bic r0, #(1<<12)		   @ - instruction cache disable
	bic r0, #(1<<2)			@ - data cache disable
	bic r0, #(1<<0)			@ - mpu disable
	mcr p15, 0, r0, c1, c0, 0  @ write control register

	ldr sp, =0x080c0000
	
	@ Flush caches
	bl flushCaches

	@ Give read/write access to all the memory regions
	ldr r0, =0x3333333
	mcr p15, 0, r0, c5, c0, 2 @ write data access
	mcr p15, 0, r0, c5, c0, 3 @ write instruction access

	@ Set MPU permissions and cache settings
	ldr r0, =0xFFFF001D @ ffff0000 32k  | bootrom (unprotected part)
	ldr r1, =0x01FF801D @ 01ff8000 32k  | itcm
	ldr r2, =0x08000029 @ 08000000 2M   | arm9 mem (O3DS / N3DS)
	ldr r3, =0x10000029 @ 10000000 2M   | io mem (ARM9 / first 2MB)
	ldr r4, =0x20000037 @ 20000000 256M | fcram (O3DS / N3DS)
	ldr r5, =0x1FF00027 @ 1FF00000 1M   | dsp / axi wram
	ldr r6, =0x1800002D @ 18000000 8M   | vram (+ 2MB)
	mov r7, #0
	mov r8, #5
	mcr p15, 0, r0, c6, c0, 0
	mcr p15, 0, r1, c6, c1, 0
	mcr p15, 0, r2, c6, c2, 0
	mcr p15, 0, r3, c6, c3, 0
	mcr p15, 0, r4, c6, c4, 0
	mcr p15, 0, r5, c6, c5, 0
	mcr p15, 0, r6, c6, c6, 0
	mcr p15, 0, r7, c6, c7, 0
	mcr p15, 0, r8, c3, c0, 0   @ Write bufferable 0, 2
	mcr p15, 0, r8, c2, c0, 0   @ Data cacheable 0, 2
	mcr p15, 0, r8, c2, c0, 1   @ Inst cacheable 0, 2

	@ Enable caches / MPU / ITCM
	mrc p15, 0, r0, c1, c0, 0  @ read control register
	orr r0, r0, #(1<<18)	   @ - ITCM enable
	orr r0, r0, #(1<<13)	   @ - alternate exception vectors enable
	orr r0, r0, #(1<<12)	   @ - instruction cache enable
	orr r0, r0, #(1<<2)		@ - data cache enable
	orr r0, r0, #(1<<0)		@ - mpu enable
	mcr p15, 0, r0, c1, c0, 0  @ write control register

	b main
