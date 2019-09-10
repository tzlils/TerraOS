CC=i686-elf-gcc
#AS=i686-elf-as
AS=nasm -f elf 
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

compile:
	$(CC) utils.c -o utils.o $(CFLAGS)
	$(AS) utils_asm.s -o utils_asm.o

clean:
	rm *.o -f
	cd ..