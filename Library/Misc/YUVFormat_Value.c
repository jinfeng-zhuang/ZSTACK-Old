#include <zstack/yuv.h>

enum YUVFormat YUVFormat_Value(const char* s)
{
	if (0 == strcmp("YU12", s)) {
		return YU12;
	}
	else if (0 == strcmp("YV12", s)) {
		return YV12;
	}
	else if (0 == strcmp("NV12", s)) {
		return NV12;
	}
	else if (0 == strcmp("NV21", s)) {
		return NV21;
	}
	else if (0 == strcmp("YU16", s)) {
		return YU16;
	}
	else if (0 == strcmp("YV16", s)) {
		return YV16;
	}
	else if (0 == strcmp("NV16", s)) {
		return NV16;
	}
	else if (0 == strcmp("NV61", s)) {
		return NV61;
	}
	else {
		return YUVFormat_NotSupport;
	}
}
