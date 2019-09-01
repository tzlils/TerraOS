#include <kernel/keyboard.h>
#include <kernel/isr.h>
#include <kernel/tty.h>
#include <kernel/vesa.h>
#include <stdio.h>
#include <string.h>

const uint8_t lower_ascii_codes[256] = {
    0x00,  ESC,  '1',  '2',     /* 0x00 */
     '3',  '4',  '5',  '6',     /* 0x04 */
     '7',  '8',  '9',  '0',     /* 0x08 */
     '-',  '=',   BS, '\t',     /* 0x0C */
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

// Scancode -> ASCII
const uint8_t upper_ascii_codes[256] = {
    0x00,  ESC,  '!',  '@',     /* 0x00 */
     '#',  '$',  '%',  '^',     /* 0x04 */
     '&',  '*',  '(',  ')',     /* 0x08 */
     '_',  '+',   BS, '\t',     /* 0x0C */
     'Q',  'W',  'E',  'R',     /* 0x10 */
     'T',  'Y',  'U',  'I',     /* 0x14 */
     'O',  'P',  '{',  '}',     /* 0x18 */
    '\n', 0x00,  'A',  'S',     /* 0x1C */
     'D',  'F',  'G',  'H',     /* 0x20 */
     'J',  'K',  'L',  ':',     /* 0x24 */
     '"',  '~', 0x00,  '|',     /* 0x28 */
     'Z',  'X',  'C',  'V',     /* 0x2C */
     'B',  'N',  'M',  '<',     /* 0x30 */
     '>',  '?', 0x00,  '*',     /* 0x34 */
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

uint8_t shift;
uint8_t ctrl;
uint8_t keypresses[256];

#define BUFFLEN 128
// New characters are added to hd. characters are pulled off of tl.
uint8_t kb_buff[BUFFLEN];
uint8_t kb_buff_hd;
uint8_t kb_buff_tl;

static void poll_keyboard_input() {
    // See if there's room in the key buffer, else bug out.
    uint8_t next_hd = (kb_buff_hd + 1) % BUFFLEN;
    if(next_hd == kb_buff_tl) {
        //return;
    }

    uint8_t byte = inb(0x60);
    if(byte == 0) {
        return;
    }

    if(byte & 0x80) {
        // Key release
        uint8_t pressedbyte = byte & 0x7F;
        // Check if we're releasing a shift key.
        if(pressedbyte == 0x2A) {
            // left
            shift = shift & 0x02;
        }
        else if(pressedbyte == 0x36) {
            // right
            shift = shift & 0x01;
        }
        else if(pressedbyte == 0x1D) {
            ctrl = 0;
        }

        keypresses[pressedbyte] = 0;
        return;
    }

    if(keypresses[byte] < 10 && keypresses[byte] > 0) {
        // Key is already pressed. Ignore it.
        keypresses[byte]++; // Increment anyway, so we can roll over and repeat.
        //return;
    }
    keypresses[byte]++;

    if(byte == 0x2A) {
        shift = shift | 0x01;
        return;
    }
    else if(byte == 0x36) {
        shift = shift | 0x02;
        return;
    }
    else if(byte == 0x1D) {
        ctrl = 1;
        return;
    }

    const uint8_t *codes;
    if(ctrl) {
        if(lower_ascii_codes[byte] == 'd') {
            // Ctrl+d
            kb_buff[kb_buff_hd] = EOT;
            kb_buff_hd = next_hd;
            return;
        }
    }
    else if(shift) {
        codes = upper_ascii_codes;
    }
    else {
        codes = lower_ascii_codes;
    }

    uint8_t ascii = codes[byte];
    //vesa_putchar(codes[byte]);
    if(ascii != 0) {
        kb_buff[kb_buff_hd] = ascii;
        kb_buff_hd = next_hd;
        return;
    }

}


void keyboard_handler(registers_t regs) {
    poll_keyboard_input();
}

void initialize_keyboard() {

    outb(0x64, 0xFF);
    register_interrupt_handler(IRQ1, keyboard_handler);

    printf("[OS] Keyboard ready\n");
}

extern void pause();
extern void sys_cli();
extern void sys_sti();

char get_ascii_char() {
    while(1) {
        sys_cli();
        if(kb_buff_hd != kb_buff_tl) {
            char c = kb_buff[kb_buff_tl];
            kb_buff_tl = (kb_buff_tl + 1) % BUFFLEN;
            poll_keyboard_input();
            sys_sti();
            return c;
        }
        sys_sti();
        pause();
    }

}