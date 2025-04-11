.section .text.start
.align 4
.arm

.global _start
_start:
    sub sp, pc, #8
    b main
