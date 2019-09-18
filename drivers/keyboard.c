#include "../include/keyboard.h"
#include "../include/irq.h"
#include "../include/stdio.h"
#include "../include/memory.h"
#include "../include/serial.h"
#include "../include/device.h"
#include "../include/kheap.h"

const uint8_t lower_ascii_codes[256] = {
    0x00,  'ESC',  '1',  '2',     /* 0x00 */
     '3',  '4',  '5',  '6',     /* 0x04 */
     '7',  '8',  '9',  '0',     /* 0x08 */
     '-',  '=',   'BS', '\t',     /* 0x0C */
     'q',  'w',  'e',  'r',     /* 0x10 */
     't',  'y',  'u',  'i',     /* 0x14 */
     'o',  'p',  '[',  ']',     /* 0x18 */
    '\n', 0x00,  'a',  's',     /* 0x1C */
     'd',  'f',  'g',  'h',     /* 0x20 */
     'j',  'k',  'l',  ';',     /* 0x24 */
    '\'',  '`', 0x00, '\\',     /* 0x28 */
     'z',  'x',  'c',  'v',     /* 0x2C */
     'b',  'n',  'm',  ',',     /* 0x30 */
     '.',  '/', 0x00,  '*',     /* 0x34 */
    0x00,  ' ', 0x00, 0x00,     /* 0x38 */
    0x00, 0x00, 0x00, 0x00,     /* 0x3C */
    0x00, 0x00, 0x00, 0x00,     /* 0x40 */
    0x00, 0x00, 0x00,  '7',     /* 0x44 */
     '8',  '9',  '-',  '4',     /* 0x48 */
     '5',  '6',  '+',  '1',     /* 0x4C */
     '2',  '3',  '0',  '.',     /* 0x50 */
    0x00, 0x00, 0x00, 0x00,     /* 0x54 */
    0x00, 0x00, 0x00, 0x00      /* 0x58 */
};

#define CALLBACK_MAX 16
void (*callbacks[CALLBACK_MAX])(uint8_t ascii) = { NULL };
int keyboard_id = 0;

extern void halt();
void keyboard_handler(struct system_frame *r) {
    uint8_t byte = inb(0x60);
    if (byte & 0x80) {
        // printf("Key released. Scan code: %x!\n", byte);
    } else {
        if(byte == 1) halt();
        for (size_t i = 0; i < keyboard_id; i++) {
            callbacks[i](lower_ascii_codes[byte]);
        }
        
        printf("%c", lower_ascii_codes[byte]);
        // printf("Key press. Scan code: %x!\n", byte);
    }
}

void register_keyboard_callback(void (*callback)(uint8_t ascii)) {
    callbacks[keyboard_id] = callback;
    keyboard_id++;
}

void init_keyboard() {
    outb(0x64, 0xFF);
    register_irq_callback (IRQ1, keyboard_handler);

    printf("[OS] Keyboard ready\n");
    
    device_t* keyboard = (device_t*)kmalloc(sizeof(device_t));
    keyboard->name = "/dev/keyboard";
    keyboard->dev_type = DEVICE_CHAR;
    int kb_id = device_add(keyboard);
}