#include <zstack/graphic.h>
#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/log.h>
#include <zstack/yuv.h>

int fill_canvas(struct canvas* canvas, u32 rgba)
{
	u32 i;
	u8 Y, U, V;
	u32 Y_size;

	// 1. check param first
	if (FALSE == check_canvas(canvas)) {
		return -1;
	}

	if (NV12 != canvas->type) {
		PRINT("format not support\n");
		return -1;
	}

	Y_size = canvas->stride * canvas->h;
	Y = rgba2yuv(rgba) & 0xFF;
	U = (rgba2yuv(rgba) >> 8) & 0xFF;
	V = (rgba2yuv(rgba) >> 16) & 0xFF;

	// 3. draw YUV: line by line > pixel by pixel
	memset(canvas->buffer, Y, Y_size);

	// 4. draw UV for NV12
	for (i = 0; i < (Y_size / 2); i+=2) {
		canvas->buffer[Y_size + i + 0] = U;
		canvas->buffer[Y_size + i + 1] = V;
	}

	return 0;
}
