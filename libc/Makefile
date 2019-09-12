CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=kernel
ASFLAGS=-f elf64

compile:
	$(CC) string.c -o string.o $(CFLAGS)
	$(CC) stdio.c -o stdio.o $(CFLAGS)

clean:
	rm *.o -f
	cd ..