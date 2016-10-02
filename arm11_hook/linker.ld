ENTRY(arm11Hook)
SECTIONS
{
    . = 0x1FFF4B40;
    .text.start : { *(.text.start) }
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss COMMON) }
    .rodata : { *(.rodata) }
    . = ALIGN(4);
}
