#include <Windows.h>
#include <zstack/zstack.h>

HWND layout_find_hwnd(struct layout* layout, const char* name)
{
	int i;

	if ((NULL == layout) || (NULL == name))
		return NULL;

	for (i = 0; '\0' != layout[i].name[0]; i++) {
		if (0 == strcmp(layout[i].name, name)) {
			return layout[i].hwnd;
		}
	}

	return NULL;
}
