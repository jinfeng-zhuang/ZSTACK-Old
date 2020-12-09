#ifndef MISC_H
#define MISC_H

#include <stddef.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////

//#ifdef _WIN32
#define OFFSET(T, f) (((unsigned int)(&(((T*)0)->f))))
//#else
//#define OFFSET(T,f) offsetof(T, f)
//#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define BITGET(RegValue, StartBit, Bits) (((RegValue) >> (StartBit)) & ((0x1 << (Bits)) - 1))

///////////////////////////////////////////////////////////////////////////////

struct application;

///////////////////////////////////////////////////////////////////////////////

extern const char *version;
extern const char *usage;

extern int hexdump(void* buffer, unsigned int length);

extern unsigned char *file_load(const char* filename, unsigned int *size);
extern int file_save(const char* filename, unsigned char* buffer, unsigned int length);
extern int file_append(const char* filename, unsigned char* buffer, unsigned int length);

extern void print_usage(void);
extern int param_parser(int argc, char *argv[], struct application *app);

extern unsigned int fsize(FILE *fp);

#endif