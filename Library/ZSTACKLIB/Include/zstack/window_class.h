#ifndef ZSTACK_WINDOW_CLASS_H
#define ZSTACK_WINDOW_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Windows.h>

enum {
    YUV_GET_INFO,
    YUV_GET_FILENAME,
    YUV_UPDATE,
};

#define YUV_STYLE_EXTERNAL  0x0001

typedef struct {
    unsigned char *data;
    unsigned int width;
    unsigned int height;
    unsigned int stride;
    unsigned int format;
} YUV_INFO;

extern unsigned int yuv_size(int width, int height, int format);
extern int draw_text(HDC hdc, const WCHAR *text, RECT *rect, COLORREF bgcolor, COLORREF fgcolor, int fontsize);

#ifdef __cplusplus
}
#endif

#endif
