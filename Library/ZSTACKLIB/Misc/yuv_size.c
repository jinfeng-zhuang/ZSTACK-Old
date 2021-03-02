#include <zstack.h>

unsigned int yuv_size(int width, int height, int format)
{
    if (format == YUV_I420) {
        return width * height * 3 / 2;
    }
    else {
        return 0;
    }
}
