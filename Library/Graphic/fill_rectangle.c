#include <zstack/graphic.h>
#include <zstack/types.h>
#include <zstack/misc.h>
#include <zstack/yuv.h>

int fill_rectangle(struct canvas* canvas, struct rect *rect_ro, u32 rgba)
{
	u32 i, j;
	struct rect rect;
	u32 offset;
	u8 Y, U, V;
	u32 Y_size;
	u16 UV;
	u32 uv_w, uv_h;
	u32 uv_x, uv_y;

	memcpy(&rect, rect_ro, sizeof(struct rect));

	// 1. check param first
	if (FALSE == check_canvas(canvas))
		return -1;

	// 2. correct the rect if it out of canvas's range
	correct_rect_on_canvas(canvas, &rect);

	if (NV12 != canvas->type) {
		return -1;
	}

	// 3. draw YUV: line by line > pixel by pixel
	Y = rgba2yuv(rgba) & 0xFF;
	for (i = 0; i < rect.h; i++) {
		offset = canvas->stride * (rect.y + i) + rect.x;
		memset(&canvas->buffer[offset], Y, rect.w);
	}

	// 4. draw UV for NV12
	uv_w = rect.w / 2; // NOTE: x can't reach rect.h, but (rect.h >> 1)
	uv_h = rect.h / 2;
	uv_x = rect.x / 2;
	uv_y = rect.y / 2;
	Y_size = canvas->stride * canvas->h;
	U = (rgba2yuv(rgba) >> 8) & 0xFF;
	V = (rgba2yuv(rgba) >> 16) & 0xFF;
	UV = (u16)(U << 8) | V;
	for (i = 0; i < uv_h; i++) {
		for (j = 0; j < uv_w; j++) {
			offset = Y_size + canvas->stride * (uv_y + i) + (uv_x + j) * 2;
			canvas->buffer[offset] = U;
			canvas->buffer[offset + 1] = V;
		}
	}

	return 0;
}
