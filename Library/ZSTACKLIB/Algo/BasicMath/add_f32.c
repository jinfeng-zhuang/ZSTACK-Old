#ifdef _WIN32

#include <math.h>
#include <float.h>

void add_f32(float *srcA, float *srcB, float *dst, unsigned int size)
{
    unsigned int i;

    for (i = 0; i < size; i++) {
        dst[i] = srcA[i] + srcB[i];
    }
}

#elif __arm__

#include "arm_math.h"

void add_f32(float *srcA, float *srcB, float *dst, unsigned int size)
{
    arm_add_f32(srcA, srcB, dst, size);
}

#endif
