#include <Windows.h>

#include  <zstack/misc.h>

extern int FieldMergeWindow_Register(HINSTANCE hInstance);
extern int FieldMergeWindowEx_Register(HINSTANCE hInstance);
extern int YUVWindow_Register(HINSTANCE hInstance);
extern int HexView_Register(HINSTANCE hInstance);

int Platform_Register(HINSTANCE hInstance)
{
    FieldMergeWindow_Register(hInstance);
    FieldMergeWindowEx_Register(hInstance);

    return 0;
}

int window_class_register(void)
{
    YUVWindow_Register(NULL);
    FieldMergeWindow_Register(NULL);
    FieldMergeWindowEx_Register(NULL);
    HexView_Register(NULL);

    return 0;
}
