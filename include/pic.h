#ifndef PIC_H
#define PIC_H
#include "../include/stdint.h"

#define MASTER_COMMAND 0x20
#define MASTER_DATA 0x21
#define SLAVE_COMMAND 0xA0
#define SLAVE_DATA 0xA1

#define PIC_RESET 0x20

void remap_pic(void);
void pic_send_eoi(uint8_t irq);

#endif