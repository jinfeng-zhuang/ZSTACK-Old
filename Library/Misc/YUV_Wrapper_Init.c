#include <zstack/yuv.h>
#include <zstack/types.h>

extern struct YUV_Wrapper yuv_wrapper_yu12;
extern struct YUV_Wrapper yuv_wrapper_yv12;
extern struct YUV_Wrapper yuv_wrapper_nv12;
extern struct YUV_Wrapper yuv_wrapper_nv21;

extern struct YUV_Wrapper yuv_wrapper_yu16;
extern struct YUV_Wrapper yuv_wrapper_yv16;
extern struct YUV_Wrapper yuv_wrapper_nv16;
extern struct YUV_Wrapper yuv_wrapper_nv61;

struct YUV_Wrapper* YUV_Wrapper_Init(enum YUVFormat format)
{
	if (YU12 == format) {
		return &yuv_wrapper_yu12;
	}
	else if (YV12 == format) {
		return NULL;
	}
	else if (NV12 == format) {
		return &yuv_wrapper_nv12;
	}
	else if (NV21 == format) {
		return NULL;
	}
	else if (YU16 == format) {
		return &yuv_wrapper_yu16;
	}
	else if (YV16 == format) {
		return NULL;
	}
	else if (NV16 == format) {
		return NULL;
	}
	else if (NV61 == format) {
		return NULL;
	}
	else {
		return NULL;
	}
}
