#include <kernel/utils.h>
#include <kernel/tty.h>


extern void halt();
int strcmp(const char *s1, const char *s2) {
    while(*s1 && *s2) {
        if(*s1 != *s2) {
            return s1 - s2;
        }
        s1++;
        s2++;
    }
    if(*s1) return 1;
    if(*s2) return -1;
    return 0;
}

void PANIC(char *err) {
    move_cursor(0, 1);
    printf("Kernel Panic - Bruh moment: %s", err);
    halt();
}