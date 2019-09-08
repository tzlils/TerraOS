CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

compile:
	$(CC) idt.c -o idt.o $(CFLAGS)
	$(CC) pic.c -o pic.o $(CFLAGS)
	$(CC) device.c -o device.o $(CFLAGS)
	$(CC) keyboard.c -o keyboard.o $(CFLAGS)
	$(CC) exceptions.c -o exceptions.o $(CFLAGS)
	$(CC) pit.c -o pit.o $(CFLAGS)
	nasm -f elf realmode.s -o realmode.o

clean:
	rm *.o -f
	cd ..