#include "nalu.h"

char *nalu_type_name(unsigned char type)
{
	if (NALU_TYPE_SLICE == type)
		return "slice";
	else if (NALU_TYPE_SLICE_A == type)
		return "slice a";
	else if (NALU_TYPE_SLICE_B == type)
		return "slice b";
	else if (NALU_TYPE_SLICE_C == type)
		return "slice c";
	else if (NALU_TYPE_IDR == type)
		return "slice idr";
	else if (NALU_TYPE_SEI == type)
		return "sei";
	else if (NALU_TYPE_SPS == type)
		return "sps";
	else if (NALU_TYPE_PPS == type)
		return "pps";
	else
		return "not support";
}
