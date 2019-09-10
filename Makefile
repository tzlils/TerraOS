CC=i686-elf-gcc
#AS=i686-elf-as
AS=nasm -f elf 
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

KERNEL_OBJS=\
boot.o \
kernel.o \
libc/string.o \
libc/stdio.o \
io/serial.o \
drivers/idt.o \
drivers/pic.o \
drivers/realmode.o \
drivers/device.o \
drivers/keyboard.o \
drivers/exceptions.o \
drivers/pit.o \
drivers/idt_asm.o \
utils/utils.o \
utils/utils_asm.o \
memory/gdt.o \
memory/malloc.o \
memory/paging.o \
memory/tasking.o \
memory/gdt_asm.o \
display/tty.o \
display/font.o \
display/vesa.o \

all: compile link iso
start: start-32

start-32:
	qemu-system-i386 terraos.iso

start-64:
	qemu-system-x86_64 terraos.iso

bochs-ui:
	bochs 'boot:cdrom' 'ata0-slave: type=cdrom, path=terraos.iso, status=inserted' 'display_library: x, options="gui_debug"'

bochs:
	bochs 'boot:cdrom' 'ata0-slave: type=cdrom, path=terraos.iso, status=inserted'

iso:
	mkdir -p isodir/boot/grub
	cp terraos.bin isodir/boot/terraos.bin
	echo -e "menuentry "TerraOS" {\nmultiboot /boot/terraos.bin\n}" >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o terraos.iso isodir
	rm -r isodir


compile:
	$(AS) boot.s -o boot.o
	$(CC) kernel.c -o kernel.o $(CFLAGS)
	cd libc && make compile
	cd io && make compile
	cd drivers && make compile
	cd utils && make compile
	cd memory && make compile
	cd display && make compile

link:
	$(CC) -T linker.ld -o terraos.bin -ffreestanding -O2 -nostdlib $(KERNEL_OBJS)  -lgcc

clean:
	cd libc && make clean
	cd io && make clean
	cd drivers && make clean
	cd utils && make clean
	cd memory && make clean
	cd display && make clean
	rm terraos.bin -f
	rm *.o -f
