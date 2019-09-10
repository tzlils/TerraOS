CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

compile:
	$(CC) malloc.c -o malloc.o $(CFLAGS)
	$(CC) gdt.c -o gdt.o $(CFLAGS)
	$(CC) paging.c -o paging.o $(CFLAGS)
	$(CC) tasking.c -o tasking.o $(CFLAGS)
	nasm -felf gdt.s -o gdt_asm.o

clean:
	rm *.o -f
	cd ..