#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>
#include <string.h>

int field_merge(unsigned char *output, unsigned char *top, unsigned char *bottom, int width, int height, int reverse)
{
    int offset_t;
    int offset_b;
    int frame_size = width * height * 2;
    int i;

    memset(output, 0, frame_size);

    if (0 == reverse) {
        offset_t = 0;
        offset_b = width;
    }
    else {
        offset_t = width;
        offset_b = 0;
    }

    for (i=0; i<height; i++) {
        memcpy(&output[width * i * 2 + offset_t], &top[width * i], width);
        memcpy(&output[width * i * 2 + offset_b], &bottom[width * i], width);
    }

    return 0;
}
