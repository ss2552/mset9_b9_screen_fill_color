ENTRY(_start)
SECTIONS
{
    . = 0x080C2340 + 0x1000;

    .text       : ALIGN(4) { *(.text.start) *(.text*); . = ALIGN(4); }
    .rodata     : ALIGN(4) { *(.rodata*); . = ALIGN(4); }
    .data       : ALIGN(4) { *(.data*); . = ALIGN(8); . = ALIGN(8); __bss_start = .; *(.bss* COMMON); . = ALIGN(8); __bss_end = .;}

    . = ALIGN(4);
}
