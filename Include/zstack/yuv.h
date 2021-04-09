#ifndef ZSTACK_YUV_H
#define ZSTACK_YUV_H

enum YUVFormat {
    YUV_I420 = 0,
    YUV_I422,
    YUV_I444,
};

struct YUVWindowStruct {
    unsigned char *buffer;
    int width;
    int height;
    int color_format; // I420
};

extern unsigned char I420_Y(int x, int y, int stride, int height, unsigned char* frame_buffer);

#endif
