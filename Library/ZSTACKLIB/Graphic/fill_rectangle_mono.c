#include <zstack/graphic.h>
#include <zstack/types.h>

int fill_rectangle(struct graphic_canvas* canvas, struct graphic_rect rect, u32 rgba)
{
	u32 i;
	u32 yuv;

	// check param first
	if (NULL == canvas)
		return -1;

	if ((canvas->w * canvas->h) > canvas->length)
		return -1;

	// correct the rect if it out of canvas's range

	rgba2yuv(rgba, &yuv);

	for (i = 0; i < (canvas->w * canvas->h); i++) {
		canvas->buffer[i] = yuv & 0xFF;
	}

	return 0;
}
