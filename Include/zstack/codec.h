#ifndef ZSTACK_CODEC_H
#define ZSTACK_CODEC_H

#include "types.h"
#include "bitstream.h"

struct sequence_header {
	u32 width;
	u32 height;
};

struct picture_header {
	u32 id;
	u8* luma;
	u8* chroma;
};

extern int decode_one_frame(u8* buffer, u32 length);

#endif
