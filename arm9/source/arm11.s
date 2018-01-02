.section .rodata
.arm

.align 5
.global arm11Hook
arm11Hook: .incbin "../build/arm11_hook.bin"
arm11HookEnd:

.align 4
.global arm11FirmlaunchStub
arm11FirmlaunchStub: .incbin "../build/arm11_firmlaunch_stub.bin"
arm11FirmlaunchStubEnd:

.align 4
.global arm11HookSize
arm11HookSize: .word arm11HookEnd - arm11Hook

.align 4
.global arm11FirmlaunchStubSize
arm11FirmlaunchStubSize: .word arm11FirmlaunchStubEnd - arm11FirmlaunchStub
