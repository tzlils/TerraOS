CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=kernel
ASFLAGS=-f elf64

compile:
	$(CC) tty.c -o tty.o $(CFLAGS)
	$(CC) font.c -o font.o $(CFLAGS)
	# $(CC) vesa.c -o vesa.o $(CFLAGS)

clean:
	rm *.o -f
	cd ..