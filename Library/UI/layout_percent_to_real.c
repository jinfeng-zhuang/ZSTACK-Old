#include <zstack/zstack.h>

void layout_percent_to_real(RECT* rect, struct layout_rect* in, struct layout_rect* out)
{
	int w, h;

	w = rect->right - rect->left;
	h = rect->bottom - rect->top;

	out->x = w * ((float)in->x / 100);
	out->y = h * ((float)in->y / 100);
	out->w = w * ((float)in->w / 100);
	out->h = h * ((float)in->h / 100);
}
