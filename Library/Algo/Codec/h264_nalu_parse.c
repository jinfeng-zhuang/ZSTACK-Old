#include <stdio.h>
#include <zstack/types.h>
#include "nalu.h"
#include "codec.h"

#define NALU_TYPE_MASK	(0x1F)

u8 * h264_nalu_parse(u8* buffer, u32 length, u32 *size, u8 *type)
{
	struct nalu* nalu;

	nalu = nalu_get_info();



	if ((0 == buffer[0]) && (0 == buffer[1]) && (0 == buffer[2]) && (1 == buffer[3])) {
		*type = buffer[4] & NALU_TYPE_MASK;
		*size = length - 4 - 1;

		nalu->type = *type;
		if (*type == 5)
			nalu->IdrPicFlag = 1;
		else
			nalu->IdrPicFlag = 0;
		nalu->nal_ref_idc = (buffer[4] >> 5) & 0x3;

		return &buffer[5];
	}
	else if ((0 == buffer[0]) && (0 == buffer[1]) && (1 == buffer[2])) {
		*type = buffer[3] & NALU_TYPE_MASK;
		*size = length - 3 - 1;

		nalu->type = *type;
		if (*type == 5)
			nalu->IdrPicFlag = 1;
		else
			nalu->IdrPicFlag = 0;
		nalu->nal_ref_idc = (buffer[4] >> 5) & 0x3;

		return &buffer[4];
	}
	else {
		return NULL;
	}
}
