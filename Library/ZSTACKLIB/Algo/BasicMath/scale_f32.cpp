#ifdef _WIN32

#include <math.h>
#include <float.h>

void scale_f32(float *src, float scale, float *dst, unsigned int size)
{
    unsigned int i;

    for (i = 0; i < size; i++) {
        dst[i] = src[i] * scale;
    }
}

#elif __arm__

#include "arm_math.h"

void scale_f32(float *src, float scale, float *dst, unsigned int size)
{
    arm_scale_f32(src, scale, dst, size);
}

#endif
