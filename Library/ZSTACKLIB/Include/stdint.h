#ifndef STDINT_H
#define STDINT_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#ifdef _WIN32
typedef unsigned long long uint64_t;
typedef long long int64_t;
#else
typedef long unsigned int __uint64_t;
typedef long int __int64_t;
typedef __uint64_t uint64_t;
typedef __int64_t int64_t;
#endif

#endif
