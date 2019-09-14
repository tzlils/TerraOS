#include "../include/keyboard.h"
#include "../include/irq.h"
#include "../include/stdio.h"
#include "../include/device.h"
#include "../include/memory.h"

void keyboard_handler(struct system_frame *r) {
    uint8_t byte = inb(0x60);
    if (byte & 0x80)
        printf("Key released. Scan code: %x!\n", byte);
    else
        printf("Key press. Scan code: %x!\n", byte);
}

void init_keyboard() {
    outb(0x64, 0xFF);
    register_irq_callback (IRQ1, keyboard_handler);

    printf("[OS] Keyboard ready\n");
    
    // device_t* keyboard = (device_t*)malloc(sizeof(device_t));
    // keyboard->name = "/dev/keyboard";
    // keyboard->dev_type = DEVICE_CHAR;
    // int kb_id = device_add(keyboard);
}