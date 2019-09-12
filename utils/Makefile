CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=kernel
ASFLAGS=-f elf64

compile:
	$(CC) utils.c -o utils.o $(CFLAGS)
	$(AS) utils_asm.s -o utils_asm.o $(ASFLAGS)

clean:
	rm *.o -f
	cd ..