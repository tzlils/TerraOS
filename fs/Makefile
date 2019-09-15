CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=kernel
ASFLAGS=-f elf64

compile:
	$(CC) ext2.c -o ext2.o $(CFLAGS)
	$(CC) vfs.c -o vfs.o $(CFLAGS);

clean:
	rm *.o -f
	cd ..