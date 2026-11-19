#!/bin/bash

as --32 boot_x86.s -o boot_x86.o
gcc -m32 -ffreestanding -O2 -c main.c -o main.o
gcc -m32 -ffreestanding -O2 -c core/shell.c -o shell.o
gcc -m32 -ffreestanding -O2 -c hal/hal_x86.c -o hal_x86.o

ld -m i386pe -N --image-base 0x100000 -T linker.ld boot_x86.o main.o shell.o hal_x86.o -o kernito_x86.exe
objcopy -O binary kernito_x86.exe kernito_x86.bin
