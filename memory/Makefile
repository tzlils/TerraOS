CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=kernel
ASFLAGS=-f elf64

compile:
	$(CC) malloc.c -o malloc.o $(CFLAGS)
	$(CC) gdt.c -o gdt.o $(CFLAGS)
	$(CC) paging.c -o paging.o $(CFLAGS)
	$(CC) tasking.c -o tasking.o $(CFLAGS)
	$(AS) gdt.s -o gdt_asm.o $(ASFLAGS)
	$(AS) paging.s -o paging_asm.o $(ASFLAGS)

clean:
	rm *.o -f
	cd ..