#include <stdio.h>
#include <zstack/codec.h>
#include <zstack/log.h>
#include "codec.h"
#include "sps.h"

void scaling_list(struct bitstream* bs, unsigned int* scalingList, unsigned int sizeOfScalingList, unsigned int* useDefaultScalingMatrixFlag)
{
	u32 lastScale = 8;
	u32 nextScale = 8;
	u32 delta_scale;

	u32 j;

	for (j = 0; j < sizeOfScalingList; j++) {
		if (nextScale != 0) {
			delta_scale = se_read_se(bs);
			nextScale = (lastScale + delta_scale + 256) % 256;
			*useDefaultScalingMatrixFlag = ((j == 0) && (nextScale == 0));
		}

		scalingList[j] = (nextScale == 0) ? lastScale : nextScale;
		lastScale = scalingList[j];
	}
}