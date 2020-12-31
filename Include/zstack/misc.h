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

#define FILE_PATH_MAX   (FILENAME_MAX)
#define IP_LENGTH       (16)

#ifndef PI
#define PI (3.141592653589793)
#endif

///////////////////////////////////////////////////////////////////////////////

struct application;

struct index_buffer {
    unsigned int total;
    unsigned int *index;
    unsigned char *data;
};

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

/*
 * Attention: str should be in array!!!
 */
extern char *trim(char *str);
extern int trim_test(void);

extern int split(char *dst, int dst_size, char *src, int src_size, char flag, int *offset);

extern int mem_format_parser_simple(const char *input, unsigned char *output);

extern long png_to_bgra(const char* path, int* w, int* h, unsigned char** bgra);

extern unsigned int count_char(unsigned char *buffer, unsigned int length, unsigned char c);

extern unsigned int replace_char(unsigned char *buffer, unsigned int length, unsigned char origin, unsigned char target);

extern int readlines(const char *filename, struct index_buffer *ib);

#endif