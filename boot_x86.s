.set ALIGN,       1<<0
.set MEMINFO,     1<<1
.set AOUT_KLUDGE, 1<<16
.set FLAGS,       ALIGN | MEMINFO | AOUT_KLUDGE
.set MAGIC,       0x1BADB002
.set CHECKSUM,    -(MAGIC + FLAGS)

.section .multiboot
.align 4
multiboot_header:
    .long MAGIC
    .long FLAGS
    .long CHECKSUM
    .long multiboot_header
    .long multiboot_header
    .long _edata
    .long _end
    .long _start

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
_start:
    mov $stack_top, %esp
    call _main
    cli
1:  hlt
    jmp 1b
