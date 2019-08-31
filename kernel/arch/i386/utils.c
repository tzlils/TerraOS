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

char* _itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

char* itoa(int i) {
    char b[256];
    _itoa(i, b);
    return b;
}

void PANIC(char *err) {
    move_cursor(0, 1);
    printf("Kernel Panic - Bruh moment: %s", err);
    halt();
}

