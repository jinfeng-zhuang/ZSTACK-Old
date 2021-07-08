#ifndef MISC_H
#define MISC_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/**************************************************************************************************
 *                                              Macros
 *************************************************************************************************/

#ifndef _WIN32
#define _fseeki64 fseek
#define _ftelli64 ftell
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define BITGET(RegValue, StartBit, Bits) (((RegValue) >> (StartBit)) & ((0x1 << (Bits)) - 1))

#define FILE_PATH_MAX   (FILENAME_MAX)
#define IP_LENGTH       (16)

#ifndef PI
#define PI (3.141592653589793)
#endif

#ifndef _WIN32
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) > (b)) ? (b) : (a))
#endif
#else
#include <stdlib.h>
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

enum YUVFormat {
	YUV_FORMAT_NV12, // YUV420: YYYY YYYY UV UV, Intel define, DirectX accelerate
	YUV_FORMAT_YV12, // YUV420: YYYY YYYY UU VV
};

/**************************************************************************************************
 *                                              Structures
 *************************************************************************************************/

struct application;

/**************************************************************************************************
 *                                              Variables
 *************************************************************************************************/

extern const char *version;
extern const char *usage;

/**************************************************************************************************
 *                                              Functions
 *************************************************************************************************/

enum {
	HEXDUMP_RAW = 0,
	HEXDUMP_ADDR = 1<<0,
	HEXDUMP_ASCII = 1<<1,
};

extern int hexdump(void* buffer, unsigned int length, u32 flags);

extern void print_usage(void);
extern int param_parser(int argc, char *argv[], struct application *app);

extern long png_to_bgra(const char* path, int* w, int* h, unsigned char** bgra);

extern void serial_port_list(void);
extern u32 serial_open(char* port);

/*
 * Block API
 * if return -1, means device read failed, if less then length, means timeout
 */
extern u32 serial_read(u32 fd, u8* buffer, u32 length);
extern u32 serial_write(u32 fd, u8* buffer, u32 length);

#endif
