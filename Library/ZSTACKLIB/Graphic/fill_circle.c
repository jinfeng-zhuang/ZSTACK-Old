#include <zstack/graphic.h>
#include <zstack/types.h>
#include <zstack/misc.h>

int fill_circle(struct canvas* canvas, struct point p, u32 r)
{
	u32 i, j;
	struct rect rect;
	u32 offset;
	u8 Y, U, V;
	u32 Y_size;
	u16 UV;
	u32 uv_w, uv_h;
	u32 uv_x, uv_y;

	// 1. check param first
	if (FALSE == check_canvas(canvas))
		return -1;

	// 2. create a circle => a function
	// 2.1 create rect
	// 2.2 for each line, determine the range
	// 2.3 fill color in the range

	// 3. copy circle to canvas => a function

	// 4. free the circle

	// 5. finally: convert to the specific format => function

	return 0;
}
