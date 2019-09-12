#include "../include/serial.h"
#include "../include/idt.h"
#include "../include/pic.h"


inline uint8_t inb(unsigned int port) {
   uint8_t ret;
  
   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
} 

inline void outb(unsigned int port,unsigned char value) {
   	asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

void set_int(int i, uint32_t addr) {
	register_interrupt_handler(i, addr, 0, 0x8e);
}

void send_eoi(uint8_t irq) {
	pic_send_eoi(irq);
}