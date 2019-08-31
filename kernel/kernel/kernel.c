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
#include <kernel/math.h>

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

void draw_hello() {
    int x_offset = 100;
    int y_offset = 200;
    draw_rect(x_offset, y_offset, 30, 100);
    draw_rect(x_offset-50, y_offset, 30, 100);
    draw_rect(x_offset-50, y_offset+40, 50, 20);


    x_offset += 90;
    draw_rect(x_offset-50, y_offset, 30, 100);
    draw_rect(x_offset-50, y_offset+80, 50, 20);
    draw_rect(x_offset-50, y_offset+40, 50, 20);
    draw_rect(x_offset-50, y_offset, 50, 20);

    x_offset += 60;
    draw_rect(x_offset-50, y_offset, 30, 100);
    draw_rect(x_offset-50, y_offset+80, 50, 20);

    x_offset += 60;
    draw_rect(x_offset-50, y_offset, 30, 100);
    draw_rect(x_offset-50, y_offset+80, 50, 20);

    x_offset += 60;
    draw_rect(x_offset, y_offset, 30, 100);
    draw_rect(x_offset-50, y_offset, 30, 100);
    draw_rect(x_offset-50, y_offset+80, 50, 20);
    draw_rect(x_offset-50, y_offset, 50, 20);

    x_offset += 45;
    draw_rect(x_offset, y_offset, 20, 70);
    draw_rect(x_offset, y_offset+80, 20, 20);
}

void kernel_main(struct multiboot_info *mi) {
	terminal_initialize();
    set_vmode(1280, 720);
    show_ver();
    init_gdt();
    remap_pic();
    init_idt();
	initialize_keyboard();

    //set_vesa_background(make_vesa_color(255, 255, 255));
    //draw_line(0, 0, 1280, 720);
    int r = 1;
    int g = 1;
    int b = 1;
    int n = 1;
    while(1) {
        //set_vesa_color(make_vesa_color(r, g, b));
        draw_hello();
        n = n + 2;
        //draw_line(0, n, n, 0);
        r = sin(g*10);
        g = cos(b*10);
        b = cos(r*10);
        pause();
    }

    // uint32_t low_pages = 256; // 1024 * 1024 bytes / 4096
    // uint32_t high_pages = (mi->mem_upper * 1024) / 4096;

    // uint32_t total_frames = high_pages + low_pages;
    //initialize_paging(total_frames, get_framebuffer_addr(), get_framebuffer_length());
    //malloc_stats();

    // for (size_t x = 0; x < 1280; x++)
    // {
    //     for (size_t y = 0; y < 720; y++)
    //     {
    //         draw_pixel_at(x, y, get_vesa_color());
    //     }
    // }
    
    move_cursor(0, 0);
    //draw_circle(0, 0, 50);
    
    

    //printf("Starting shell...\n");
    //shell();
    //printf("You can now safely abort the system");
}

void draw_fibbonachi() {

}

void draw_rect(int x, int y, int w, int h) {
    for (size_t i = y; i < y+h; i++)
    {
        for (size_t j = x; j < x+w; j++) {
            draw_pixel_at(j, i, get_vesa_color());
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2) {
    int w = x2 - x1;
    int h = y2 - y1;
    double m = h/(double)w;
    double j = y1;
    for (size_t i = x1; i <= x2; i++)
    {
        draw_pixel_at(i, (int)j, get_vesa_color());
        j += m;
    }
}

void draw_circle(int x, int y, int r) {
    double slice = 2 * PI / 8;
    for (size_t i = 0; i < 8; i++)
    {
        double angle = slice * i;
        int px = x+r*cos(angle);
        int py = y+r+sin(angle);
        draw_pixel_at(px, py, get_vesa_color());
    }
}
