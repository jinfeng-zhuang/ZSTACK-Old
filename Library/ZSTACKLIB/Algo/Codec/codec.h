#ifndef _ZSTACK_CODEC_
#define _ZSTACK_CODEC_

#include <zstack/bitstream.h>
#include <zstack/types.h>

extern int nalu_get(u8* buffer, u32 length, u32* offset, u32* size);
extern u8* h264_nalu_parse(u8* buffer, u32 length, u32* size, u8* type);

extern int sps_parse(struct seq_parameter_set_rbsp* sps, unsigned char* buffer, unsigned int length);
extern struct seq_parameter_set_rbsp* sps_get(unsigned int id);

extern void scaling_list(struct bitstream* bs, unsigned int* scalingList, unsigned int sizeOfScalingList, unsigned int* useDefaultScalingMatrixFlag);

#endif
