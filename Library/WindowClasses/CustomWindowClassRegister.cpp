#include <Windows.h>

#include  <zstack/misc.h>

extern int FieldMergeWindow_Register(HINSTANCE hInstance);

int Platform_Register(HINSTANCE hInstance)
{
    FieldMergeWindow_Register(hInstance);

    return 0;
}
