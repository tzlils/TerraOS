CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

compile:
	$(CC) idt.c -o idt.o $(CFLAGS)
	$(CC) pic.c -o pic.o $(CFLAGS)
	$(AS) idt_asm.s -o idt_asm.o
	$(CC) isr.c -o isr.o $(CFLAGS)
	$(CC) device.c -o device.o $(CFLAGS)
	$(CC) keyboard.c -o keyboard.o $(CFLAGS)
	nasm -f elf realmode.s -o realmode.o

clean:
	rm *.o -f
	cd ..