#include <kernel/shell.h>
#include <kernel/tty.h>
#include <stdint.h>
#include <kernel/utils.h>

;
int scan_command(char *buffer, char **comm, char **fname) {
    char *buffscan = buffer;
    if(!*buffscan) {
        return 0;
    }

    while(*buffscan && *buffscan == ' ') {
        buffscan++;
    }

    if(!*buffscan) {
        return 0;
    }

    *comm = buffscan;

    while(*buffscan && *buffscan != ' ') {
        buffscan++;
    }

    if(!*buffscan) {
        return 1;
    }
    *buffscan = 0;
    buffscan++;

    while(*buffscan && *buffscan == ' ') {
        buffscan++;
    }

    if(!*buffscan) {
        return 1;
    }

    *fname = buffscan;

    while(*buffscan && *buffscan != ' ') {
        buffscan++;
    }
    *buffscan = 0;

    return 2;
}

void help() {
    printf("Commands:\n");
    printf("echo [text] => prints text\n");
    printf("exit => gracefully exists the shell\n");
}

void shell() {
    printf("Shell successfully opened, type ? for commands\n");
    uint32_t bufflen = 24;
    char buffer[bufflen+1];
    while(1) {
        printf(">");
        uint32_t i;
        for(i = 0; i < bufflen; i++) {
            char c = get_ascii_char();
            if(c == BS) {
                if(i == 0) {
                    i--;
                    continue;
                }
                printf("%c", c);
                i-=2;
                continue;
            }
            if(c == EOT || c == ESC) {
                i--;
                continue;
            }
            printf("%c", c);

            buffer[i] = c;
            if(c == '\n') break;
        }

        int command_ret = handle_commands(buffer);
        if(command_ret == -1) {
            break;
        } continue;
    }

    printf("Exiting shell...\n");
}

int handle_commands(char* buffer) {
    char *command;
    char *params;
    if(scan_command(buffer, &command, &params) == 0) {
        printf("failed to parse command\n");
        return 0;
    }

    int ret = 0;
    if(strcmp(command, "echo") == 1) {
        printf(params);
        printf("\n");
        ret = 1;
    } else if(strcmp(command, "exit") == 1) {
        ret = -1;
    } else if(strcmp(command, "?") == 1) {
        help();
        ret = 1;
    } else {
        printf("unknown command.\n");
        ret = 1;
    }

    return ret;
}