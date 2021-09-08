#include <zstack/graphic.h>

int correct_rect_on_canvas(struct canvas* canvas, struct rect* rect)
{
	if ((NULL == canvas) || (NULL == rect))
		return -1;

	if ((rect->x >= canvas->w) || (rect->y >= canvas->h))
		return 0;

	if (rect->x + rect->w > canvas->w) {
		rect->w = canvas->w - rect->x;
	}
	if (rect->y + rect->h > canvas->h) {
		rect->h = canvas->h - rect->y;
	}

	return 0;
}