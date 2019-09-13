#include "../include/serial.h"

inline uint8_t inb(unsigned int port) {
   uint8_t ret;

   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}

inline void outb(unsigned int port,unsigned char value) {
   	asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}
