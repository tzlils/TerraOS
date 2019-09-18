#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../include/stdint.h"

void init_keyboard();
void register_keyboard_callback(void (*callback)(uint8_t ascii));

#endif