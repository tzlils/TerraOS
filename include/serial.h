#ifndef SERIAL_H
#define SERIAL_H
#include "../include/stdint.h"

uint8_t inb(unsigned int port);
void outb(unsigned int port,unsigned char value);
void set_int(int i, uint32_t addr);
void send_eoi(uint8_t irq);
#endif