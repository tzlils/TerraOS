#include <stdint.h>
#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
 
#include <stddef.h>

#define ESC (0x1B)
#define BS  (0x08)
#define EOT (0x04)
 
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_setcolor(uint8_t color);
void terminal_setfg(int color);
void terminal_setbg(int color);
void move_cursor(uint8_t xpos, uint8_t ypos);
void terminal_set_status();

#endif