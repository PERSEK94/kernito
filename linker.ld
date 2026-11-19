ENTRY(_start)
SECTIONS
{
    . = 0x100000;

    .text 0x100000 : {
        *(.multiboot)
        *(.text*)
        *(.rodata*)
        *(.rdata*)
        *(.data*)
    }

    _edata = .;

    .bss : {
        *(COMMON)
        *(.bss*)
    }

    _end = .;

    /DISCARD/ : {
        *(.reloc*)
        *(.comment*)
        *(.eh_frame*)
    }
}
