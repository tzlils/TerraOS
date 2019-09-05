CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

compile:
	$(CC) tty.c -o tty.o $(CFLAGS)
	$(CC) font.c -o font.o $(CFLAGS)
	$(CC) vesa.c -o vesa.o $(CFLAGS)

clean:
	rm *.o -f
	cd ..