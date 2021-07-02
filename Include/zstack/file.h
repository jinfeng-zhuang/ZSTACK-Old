// should support >4GB file

#ifndef ZSTACK_FILE_H
#define ZSTACK_FILE_H

#include "types.h"

// should support unicode text
extern u32 read_one_line(const char *filename, u64 *filepos, u8 *buffer);

extern u64 file_size(const char* filename);

// if file size  >500MB, should not use this, or every time, load the limited size content
// size can specify the size, -1 = all, 0 = all
// offset can specify, will be updated by ftell internal
extern u8 *file_load(const char* filename, u64 *offset, u64 *size);

extern int file_save(const char* filename, u8* buffer, unsigned int length);
extern int file_append(const char* filename, u8* buffer, unsigned int length);

#endif