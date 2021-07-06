#ifndef ZSTACK_GRAPHIC_H
#define ZSTACK_GRAPHIC_H

#include <zstack/types.h>

struct graphic_canvas {
	u8 *buffer;
	u32 length;
	u32 w;
	u32 h;
	u32 stride;
	u8 type;
};

struct graphic_point {
	u32 x;
	u32 y;
};

struct graphic_rect {
	u32 x;
	u32 y;
	u32 w;
	u32 h;
};

extern int fill_rectangle(struct graphic_canvas *canvas, struct graphic_rect rect, u32 rgba);

// transformation
extern void rgba2yuv(u32 rgba, u32 *yuv);
extern void yuv2rgba(u32 yuv, u32 *rgba);

extern int transform_location_to_address(u8 *buffer, u32 length, u32 x, u32 y);

#endif
