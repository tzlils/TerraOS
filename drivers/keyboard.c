#include "../include/keyboard.h"
#include "../include/idt.h"
#include "../include/stdio.h"
#include "../include/device.h"
#include "../include/memory.h"
#include "../include/serial.h"

void keyboard_handler() {
    printf("Key clicked!\n");
    uint8_t byte = inb(0x60);
}

void init_keyboard() {
    outb(0x64, 0xFF);
    idt_register_interrupt(IRQ1, keyboard_handler);

    printf("[OS] Keyboard ready\n");
    
    device_t* keyboard = (device_t*)malloc(sizeof(device_t));
    keyboard->name = "/dev/keyboard";
    keyboard->dev_type = DEVICE_CHAR;
    int kb_id = device_add(keyboard);
}