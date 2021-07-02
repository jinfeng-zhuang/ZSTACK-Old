#ifndef ZSTACK_STRING_H
#define ZSTACK_STRING_H

#include "types.h"

extern u32 char_count(u8 *buffer, u32 length, u8 c);
extern u32 char_replace(u8 *buffer, u32 length, u8 c, u8 r);

// remove space from head and tail
extern char *trim(char *str);

// split string with specified char
extern int split(char *dst, int dst_size, char *src, int src_size, char flag, int *offset);

#endif