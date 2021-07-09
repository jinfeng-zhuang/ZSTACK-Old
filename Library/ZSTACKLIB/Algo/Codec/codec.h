#ifndef _ZSTACK_CODEC_
#define _ZSTACK_CODEC_

#include <zstack/bitstream.h>

extern unsigned int u(struct bitstream* bitstream, unsigned int num);
extern unsigned int ue(struct bitstream* bitstream);
extern int se(struct bitstream* bitstream);
extern int more_rbsp_data(struct bitstream* bitstream);
extern int rbsp_trailing_bits(struct bitstream* bitstream);
extern int bitstream_error(struct bitstream* bitstream);
extern int is_byte_aligned(struct bitstream* bitstream);

#endif
