#ifndef ZSTACK_WINDOW_CLASS_H
#define ZSTACK_WINDOW_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Windows.h>

// YUVWindow

enum YUVFormat {
    YUV_I420 = 0,
    YUV_I422,
    YUV_I444,
};

struct YUVWindowData {
    unsigned int width;
    unsigned int height;
    unsigned int stride;
    unsigned int format; // enum YUVFormat
    unsigned char *data;
};

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
