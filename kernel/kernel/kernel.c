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
#include <kernel/vesa.h>
#include <kernel/utils.h>
#include <kernel/math.h>
#include <kernel/draw.h>
#include <kernel/kheap.h>
#include <kernel/paging.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

extern void pause();
extern void abort();
extern char _binary_f32_disk_start;

void show_ver() {
    set_vesa_color(make_vesa_color(255, 0, 0));
    set_vesa_background(make_vesa_color(0, 255, 0));
	// printf(L"  _____                    ___  ____  \n");
	// printf(L" |_   _|__ _ __ _ __ __ _ / _ \/ ___| \n");
	// printf(L"   | |/ _ \ '__| '__/ _` | | | \___ \ \n");
	// printf(L"   | |  __/ |  | | | (_| | |_| |___) |\n");
	// printf(L"   |_|\___|_|  |_|  \__,_|\___/|____/ \n");
	printf("TerraOS V1.0\n", 0);
    set_vesa_color(make_vesa_color(255, 255, 255));
    set_vesa_background(make_vesa_color(0, 0, 0));
}

void kernel_main(struct multiboot_info *mi) {
	terminal_initialize();
    set_vmode(1280, 720);

    // uint32_t low_pages = 256; // 1024 * 1024 bytes / 4096
    // uint32_t high_pages = (mi->mem_upper * 1024) / 4096;

    // uint32_t total_frames = high_pages + low_pages;
    // initialize_paging(total_frames, get_framebuffer_addr(), get_framebuffer_length());
    // malloc_stats();


    show_ver();
    init_gdt();
    remap_pic();
    init_idt();
    initialize_kheap(256);
	initialize_keyboard();

    printf ("Log2 of 10: %d\n", log2(10));
    printf ("2 To the power of 16: %i\n", pow(2, 16));
    printf("Max of 1, 5: %i\n", max(1, 5));
    printf("Sin of 5: %d\n", sin(5));

    //set_vesa_background(make_vesa_color(255, 255, 255));
    int angle = 0;
    double x, y;

    draw_hello();
    //draw_polygon(3, *Vector2_new(1280/2, 720/2), *Vector2_new(1280/2-100, 720/2-100), *Vector2_new(1280/2+100, 720/2+100));
    draw_line(*Vector2_new(0, 720/2), *Vector2_new(1280, 720/2));
    for (size_t i = 0; i < 1280; i+=20)
    {
        draw_rect(*Vector2_new(i, 720/2-5), 2, 10);
    }
    
    for (x = 0; x < 1280; x+=3)
    {
        y = 50*sin(angle*PI/180);
        y = 720/2 - y;
        draw_pixel_at(x, y, get_vesa_color());
        angle+=5;

        pause();
    }
    while(1);

    // for (size_t x = 0; x < 1280; x++)
    // {
    //     for (size_t y = 0; y < 720; y++)
    //     {
    //         draw_pixel_at(x, y, get_vesa_color());
    //     }
    // }
    
    // move_cursor(0, 0);
    
    

    // printf("Starting shell...\n");
    // shell();
    // printf("You can now safely abort the system");
}