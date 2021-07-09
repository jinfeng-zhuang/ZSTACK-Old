#ifndef _ZSTACK_SCALING_LIST_H_
#define _ZSTACK_SCALING_LIST_H_

struct scaling_list {
	unsigned int ScalingList4x4[6][16];
	unsigned int ScalingList8x8[6][64];
	unsigned int UseDefaultScalingMatrix4x4Flag[6];
	unsigned int UseDefaultScalingMatrix8x8Flag[6];
};

#endif