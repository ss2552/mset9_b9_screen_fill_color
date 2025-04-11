.section .text.start
.align 4
.arm

.global _start
_start:
    mov sp, pc
    b main
