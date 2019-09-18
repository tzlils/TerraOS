#include "../include/stdio.h"
#include "../include/keyboard.h"
#include "../include/stdint.h"
#include "../include/string.h"
#include "../include/tty.h"
#include "../include/utils.h"
#include "../include/vfs.h"

char command_buffer[256];
int command_pos;

void handle_input(uint8_t ascii) {
    if(ascii == '\n') {
        do_command();
        clear_command_buffer();
        return;
    }

    command_buffer[command_pos] = ascii;
    command_pos++;
}

void do_command() {
    char *command = kmalloc(strlen(command_buffer) + 1);
	memcpy(command, command_buffer, strlen(command_buffer) +1);
    char* arguments[16];

    size_t n = strsplit(command, ' ');
    int argument_length = n;

    for (size_t i = 0; i < n; i++) {
        arguments[i] = command;
        
        uint32_t s = strlen(command);
        command += s + 1;
    }

    char *cmd = arguments[0];


    printf("\n");
    if(strcmp(cmd, "echo") == 0) {
        for (size_t i = 1; i < argument_length; i++)
        {
            printf("%s ", arguments[i]);
        }
    } else if(strcmp(cmd, "credits") == 0) {
        printf("TerraOS was written by Terradice");
    } else if(strcmp(cmd, "mounts") == 0) {
        list_mount();
    } else if(strcmp(cmd, "touch")) {
        printf("Created file %s ", arguments[1]);
        vfs_touch(arguments[1]);
    } else if(strcmp(cmd, "ls") == 0) {
        char* buf;
        vfs_ls("/", buf);
        printf("%s", buf);
    }
}

void clear_command_buffer() {
    for (size_t i = 0; i < 256; i++)
    {
        command_buffer[i] = 0;
    }
    command_pos = 0;
}

void init_shell() {
    register_keyboard_callback(handle_input);
    printf("Shell is ready, command buffer length is 256\n");
}