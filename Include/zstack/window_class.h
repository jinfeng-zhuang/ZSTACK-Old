#ifndef ZSTACK_WINDOW_CLASS_H
#define ZSTACK_WINDOW_CLASS_H

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
    enum YUVFormat format;
    unsigned char *data;
};

#endif
