// should support >4GB file

#ifndef ZSTACK_FILE_H
#define ZSTACK_FILE_H

#include "types.h"

extern u64 file_size(const char* filename);
extern u32 file_load(const char* filename, u64 offset, u8 *buffer, u32 length);
extern u32 file_save(const char* filename, u8* buffer, u32 length);
extern u32 file_append(const char* filename, u8* buffer, u32 length);

#endif
