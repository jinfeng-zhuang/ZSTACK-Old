#ifndef ZSTACK_GRAPHIC_H
#define ZSTACK_GRAPHIC_H

#include <zstack/types.h>

#define COLOR_WHITE		0xFFFFFF
#define COLOR_BLACK		0x000000
#define COLOR_YELLOW	0
#define COLOR_RED		0
#define COLOR_GREEN		0
#define COLOR_BLUE		0

struct canvas {
	u8 *buffer;
	u32 length;
	u32 w;
	u32 h;
	u32 stride;
	u8 type;
};

struct point {
	u32 x;
	u32 y;
};

struct rect {
	u32 x;
	u32 y;
	u32 w;
	u32 h;
};

/*****************************************************************************
 * Draw
 ****************************************************************************/

extern int fill_rectangle(struct canvas *canvas, struct rect *rect, u32 rgba);
extern int fill_circle(struct canvas* canvas, struct rect* rect, u32 rgba);

extern int draw_rectangle(struct canvas* canvas, struct rect* rect, u32 rgba);
extern int draw_line(struct canvas* canvas, struct rect* rect, u32 rgba);
extern int draw_circle(struct canvas* canvas, struct rect* rect, u32 rgba);

/*****************************************************************************
 * Internal Functions
 ****************************************************************************/

/*****************************************************************************
 * transformation
 ****************************************************************************/
extern u32 rgba2yuv(u32 rgba);
extern u32 yuv2rgba(u32 yuv);

extern u32 canvas_y_offset(struct canvas* canvas, struct point* point);
extern u32 canvas_uv_offset(struct canvas* canvas, struct point* point);
extern u32 canvas_rgb_offset(struct canvas* canvas, struct point* point);
extern u32 canvas_rgba_offset(struct canvas* canvas, struct point* point);

extern int set_y(struct canvas* canvas, struct point* point, u32 rgba);
extern int set_uv(struct canvas* canvas, struct point* point, u32 rgba);
extern int set_yuv(struct canvas* canvas, struct point* point, u32 rgba);
extern int set_rgb(struct canvas* canvas, struct point* point, u32 rgba);
extern int set_rgba(struct canvas* canvas, struct point* point, u32 rgba);

// 0% ~ 100%
extern u32 rgba_percent(u8 r, u8 g, u8 b, u8 alpha);

extern int transform_location_to_address(u8 *buffer, u32 length, u32 x, u32 y);

/*****************************************************************************
 * Helper
 ****************************************************************************/
extern int check_canvas(struct canvas* canvas);
extern int correct_rect_on_canvas(struct canvas* canvas, struct rect* rect);

#endif
