.section .text
.align 4
.arm

.global arm9Payload
arm9Payload: .incbin "build/arm9.bin"
arm9PayloadEnd:

.align 4
.global arm9PayloadSize
arm9PayloadSize: .word arm9PayloadEnd - arm9Payload
