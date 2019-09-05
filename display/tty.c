#include "../include/stdbool.h"
#include "../include/string.h"
#include "../include/tty.h"
#include "../include/vga.h"
// #include "../include/vesa.h"
#include "../include/serial.h"

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT    0x3D5
#define VGA_CURSOR_HIGH  14
#define VGA_CURSOR_LOW   15
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;
int fg_color;
int bg_color;
 
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;
 
void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	fg_color = VGA_COLOR_LIGHT_GREY;
	bg_color = VGA_COLOR_BLACK;
	terminal_color = vga_entry_color(fg_color, bg_color);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

static void terminal_scroll()
{
    uint16_t blank = vga_entry(' ', terminal_color);
    //Move the lines up
    for(unsigned int i = VGA_WIDTH; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        terminal_buffer[i] = terminal_buffer[i+VGA_WIDTH];
    }

    //Clear the last line
    for(unsigned int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
    {
        terminal_buffer[i] = blank;
        terminal_column = 0;
    }
}


void terminal_setfg(int color) {
	terminal_color = vga_entry_color(color, bg_color);
};

void terminal_setbg(int color) {
	terminal_color = vga_entry_color(fg_color, color);
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void move_cursor(uint8_t xpos, uint8_t ypos)
{
    uint16_t location = ypos * VGA_WIDTH + xpos;

    outb(VGA_COMMAND_PORT, VGA_CURSOR_HIGH); 
    outb(VGA_DATA_PORT, location >> 8);
    outb(VGA_COMMAND_PORT, VGA_CURSOR_LOW);
    outb(VGA_DATA_PORT, location);
}

static void terminal_newline()
{
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
    {
        terminal_row--;
        terminal_scroll();
    }
    move_cursor(terminal_column, terminal_row);
}

static void terminal_advance()
{
    if ( ++terminal_column == VGA_WIDTH)
    {
        terminal_newline();
    }
    else
    {
        move_cursor(terminal_column, terminal_row);
    }
}



void terminal_putchar(char c)
{
    switch (c)
    {
    case '\n':
        terminal_newline();
        break;
    case '\t':
        terminal_writestring("    ");
        break;
    case '\b':
        if(terminal_column > 0) {
            terminal_column--;
            terminal_putchar(' ');
            terminal_column--;
        }
        break;
    default:
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        terminal_advance();
        break;
    }
}
 
void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
        #ifndef VESA_H
		terminal_putchar(data[i]);
        #else
        vesa_putchar(data[i]);
        #endif
}
 
void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_set_status() {
    
}