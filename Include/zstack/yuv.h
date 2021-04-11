#ifndef ZSTACK_YUV_H
#define ZSTACK_YUV_H

enum YUVFormat {
    YUV_I420 = 0,
    YUV_I422,
    YUV_I444,
};

extern unsigned char I420_Y(int x, int y, int stride, int height, unsigned char* frame_buffer);

#endif
