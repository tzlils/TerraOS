CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=kernel -O3
ASFLAGS=-f elf64
LINKERFLAGS=-z max-page-size=0x1000 -Wl,--build-id=none -ffreestanding -O3 -nostdlib

KERNEL_OBJS=\
boot.o \
kernel.o \
libc/string.o \
libc/stdio.o \
io/serial.o \
drivers/idt.o \
drivers/pic.o \
drivers/device.o \
drivers/keyboard.o \
drivers/exceptions.o \
drivers/pit.o \
drivers/idt_asm.o \
drivers/irq.o \
utils/utils.o \
utils/utils_asm.o \
drivers/isr.o \
memory/gdt.o \
memory/malloc.o \
memory/paging.o \
memory/tasking.o \
memory/gdt_asm.o \
memory/paging_asm.o \
display/tty.o \
display/font.o \

#drivers/realmode.o
#display/vesa.o
all: compile link iso
start: start-32

start-32:
	qemu-system-i386 terraos.iso

start-64:
	qemu-system-x86_64 terraos.iso

bochs-ui:
	bochs 'magic_break: enabled=1' 'boot:cdrom' 'ata0-slave: type=cdrom, path=terraos.iso, status=inserted' 'display_library: x, options="gui_debug"'

bochs:
	bochs 'magic_break: enabled=1' 'boot:cdrom' 'ata0-slave: type=cdrom, path=terraos.iso, status=inserted'

iso:
	mkdir -p isodir/boot/grub
	cp terraos.bin isodir/boot/terraos.bin
	echo -e "menuentry "TerraOS" {\nmultiboot /boot/terraos.bin\n}" >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o terraos.iso isodir
	rm -r isodir


compile:
	$(AS) boot.s -o boot.o $(ASFLAGS)
	$(CC) kernel.c -o kernel.o $(CFLAGS)
	cd libc && make compile
	cd io && make compile
	cd drivers && make compile
	cd utils && make compile
	cd memory && make compile
	cd display && make compile

link:
	$(CC) -T linker.ld -o terraos.bin $(LINKERFLAGS) $(KERNEL_OBJS)  -lgcc

clean:
	cd libc && make clean
	cd io && make clean
	cd drivers && make clean
	cd utils && make clean
	cd memory && make clean
	cd display && make clean
	rm terraos.bin -f
	rm *.o -f