#ifndef _ZSTACK_CODEC_
#define _ZSTACK_CODEC_

#include <zstack/bitstream.h>
#include <zstack/types.h>
#include "sps.h"
#include "pps.h"
#include "nalu.h"

extern int se_read_se(struct bitstream* bs);
extern unsigned int se_read_u(struct bitstream* bs, unsigned int num);
extern unsigned int se_read_ue(struct bitstream* bs);

extern int nalu_get(u8* buffer, u32 length, u32* offset, u32* size);
extern u8* h264_nalu_parse(u8* buffer, u32 length, u32* size, u8* type);

extern int sps_parse(struct seq_parameter_set_rbsp* sps, unsigned char* buffer, unsigned int length);
extern struct seq_parameter_set_rbsp* sps_get(unsigned int id);
extern void sps_dump(struct seq_parameter_set_rbsp* sps);

extern void scaling_list(struct bitstream* bs, unsigned int* scalingList, unsigned int sizeOfScalingList, unsigned int* useDefaultScalingMatrixFlag);

extern int pps_parse(struct pic_parameter_set_rbsp* pps, unsigned char* buffer, unsigned int length);
extern void pps_dump(struct pic_parameter_set_rbsp* pps);
extern struct pic_parameter_set_rbsp* pps_get(unsigned int id);

extern int more_rbsp_data(struct bitstream* bs);
extern int rbsp_trailing_bits(struct bitstream* bs);

extern struct nalu* nalu_get_info(void);

#endif
