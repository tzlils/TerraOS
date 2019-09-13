CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=kernel
ASFLAGS=-f elf64

compile:
	$(CC) idt.c -o idt.o $(CFLAGS)
	$(CC) pic.c -o pic.o $(CFLAGS)
	$(CC) device.c -o device.o $(CFLAGS)
	$(CC) keyboard.c -o keyboard.o $(CFLAGS)
	$(CC) exceptions.c -o exceptions.o $(CFLAGS)
	$(CC) pit.c -o pit.o $(CFLAGS)
	# $(AS) realmode.s -o realmode.o $(ASFLAGS)
	$(AS) idt.s -o idt_asm.o $(ASFLAGS)
	$(CC) irq.c -o irq.o $(CFLAGS)
	$(CC) isr.c -o isr.o $(CFLAGS)

clean:
	rm *.o -f
	cd ..
