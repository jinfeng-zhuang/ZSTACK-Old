#ifdef _WIN32

#include <math.h>
#include <float.h>

void offset_f32(float *src, float offset, float *dst, unsigned int size)
{
    unsigned int i;

    for (i = 0; i < size; i++) {
        dst[i] = src[i] + offset;
    }
}

#elif __arm__

#include "arm_math.h"

void offset_f32(float *src, float offset, float *dst, unsigned int size)
{
    arm_add_f32(srcA, srcB, dst, size);
}

#endif
