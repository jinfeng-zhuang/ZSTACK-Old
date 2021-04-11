#ifndef ZSTACK_WINDOW_CLASS_H
#define ZSTACK_WINDOW_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Windows.h>

enum {
    WIN_YUV_RESOLUTION,
    WIN_YUV_FORMAT,
    WIN_YUV_INDEX
};

extern unsigned int yuv_size(int width, int height, int format);
extern int draw_text(HDC hdc, const WCHAR *text, RECT *rect, COLORREF bgcolor, COLORREF fgcolor, int fontsize);

#ifdef __cplusplus
}
#endif

#endif
