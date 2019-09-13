#ifndef SERIAL_H
#define SERIAL_H
#include "../include/stdint.h"

uint8_t inb(unsigned int port);
void outb(unsigned int port,unsigned char value);
void send_eoi(uint8_t irq);
#endif
