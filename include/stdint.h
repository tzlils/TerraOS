#ifndef STDINT_H
#define STDINT_H

typedef signed char       int8_t;
typedef signed short      int16_t;
typedef signed int        int32_t;
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned long int uint64_t;

typedef long int __intmax_t;
typedef unsigned long int __uintmax_t;

typedef __intmax_t		intmax_t;
typedef __uintmax_t		uintmax_t;

#define __WINT_TYPE__ unsigned int
typedef __WINT_TYPE__ wint_t;

#endif