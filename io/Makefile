CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

compile:
	$(CC) serial.c -o serial.o $(CFLAGS)

clean:
	rm *.o -f
	cd ..