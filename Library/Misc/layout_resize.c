#include <Windows.h>
#include <zstack/zstack.h>

void layout_resize(HWND hWnd, struct layout* layout)
{
	HWND hwnd;
	RECT rc;
	struct layout_rect lr = { 0 };
	int i;

	GetClientRect(hWnd, &rc);

	for (i = 0; '\0' != layout[i].name[0]; i++) {
		layout_percent_to_real(&rc, &layout[i].rect, &lr);

		MoveWindow(layout[i].hwnd, lr.x, lr.y, lr.w, lr.h, TRUE);
	}
}