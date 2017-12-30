.section .text
.align 4
.arm


.global arm11Hook
arm11Hook: .incbin "../build/arm11_hook.bin"
arm11HookEnd:

.align 4
.global arm11HookSize
arm11HookSize: .word arm11HookEnd - arm11Hook
