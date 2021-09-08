#include <zstack/graphic.h>
#include <zstack/misc.h>

int check_canvas(struct canvas* canvas)
{
	if (NULL == canvas)
		return FALSE;

	if ((canvas->w * canvas->h) > canvas->length)
		return FALSE;

	return TRUE;
}