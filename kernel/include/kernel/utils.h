#include <stdint.h>
int strcmp(const char *cs, const char *ct);
void PANIC(char *err);
extern void fastcp(char *dest, char *src, uint32_t count);
char* itoa(int i);