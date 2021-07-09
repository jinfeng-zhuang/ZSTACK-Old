#include <stdio.h>
#include <zstack/types.h>
#include "nalu.h"

#define NALU_TYPE_MASK	(0x1F)

u8 * h264_nalu_parse(u8* buffer, u32 length, u32 *size, u8 *type)
{
	if ((0 == buffer[0]) && (0 == buffer[1]) && (0 == buffer[2]) && (1 == buffer[3])) {
		*type = buffer[4] & NALU_TYPE_MASK;
		*size = length - 4 - 1;
		return &buffer[5];
	}
	else if ((0 == buffer[0]) && (0 == buffer[1]) && (1 == buffer[2])) {
		*type = buffer[3] & NALU_TYPE_MASK;
		*size = length - 3 - 1;
		return &buffer[4];
	}
	else {
		return NULL;
	}
}
