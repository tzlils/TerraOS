#include <stdio.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/keyboard.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/multiboot.h>
#include <kernel/frame.h>
#include <kernel/shell.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

extern void pause();
extern char _binary_f32_disk_start;

void show_ver() {
    terminal_setfg(3);
	terminal_setbg(4);
	// printf(L"  _____                    ___  ____  \n");
	// printf(L" |_   _|__ _ __ _ __ __ _ / _ \/ ___| \n");
	// printf(L"   | |/ _ \ '__| '__/ _` | | | \___ \ \n");
	// printf(L"   | |  __/ |  | | | (_| | |_| |___) |\n");
	// printf(L"   |_|\___|_|  |_|  \__,_|\___/|____/ \n");
	printf("TerraOS V1.0\n", 0);
	terminal_setfg(7);
	terminal_setbg(0);
}

void kernel_main(struct multiboot_info *mi) {
	terminal_initialize();
    show_ver();
    uint32_t low_pages = 256; // 1024 * 1024 bytes / 4096
    uint32_t high_pages = (mi->mem_upper * 1024) / 4096;

    uint32_t total_frames = high_pages + low_pages;

    printf("[OS] Starting initialization \n");
    init_gdt();
    remap_pic();
    init_idt();
	initialize_keyboard();

    //printf("Starting shell...\n");
    shell();
    //printf("You can now safely abort the system");
}