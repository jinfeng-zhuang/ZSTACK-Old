#include <Windows.h>
#include <zstack/zstack.h>

void layout_create(HWND hWnd, struct layout* layout)
{
	RECT rc;
	struct layout_rect lr = { 0 };
	int i;
	HINSTANCE hInstance;

	if ((NULL == layout) || (NULL == hWnd)) {
		warn("param shouldn't be NULL\n");
		return;
	}

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	GetClientRect(hWnd, &rc);

	for (i = 0; '\0' != layout[i].name[0]; i++) {
		layout_percent_to_real(&rc, &layout[i].rect, &lr);

		layout[i].hwnd = CreateWindowEx(
			0,
			layout[i].classname,
			layout[i].name,
			WS_CHILD | WS_VISIBLE | layout[i].style,
			lr.x, lr.y, lr.w, lr.h,
			hWnd,
			NULL,
			NULL,
			NULL);
	}

}