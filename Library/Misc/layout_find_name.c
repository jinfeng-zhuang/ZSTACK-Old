#include <Windows.h>
#include <zstack/zstack.h>

const char *layout_find_name(struct layout* layout, HWND hwnd)
{
	int i;

	if ((NULL == layout) || (NULL == hwnd))
		return NULL;

	for (i = 0; '\0' != layout[i].name[0]; i++) {
		if (hwnd == layout[i].hwnd) {
			return layout[i].name;
		}
	}

	return NULL;
}
