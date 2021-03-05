#ifdef _WIN32

#include <math.h>
#include <float.h>

void mean_f32(float *src, unsigned int size, float *result)
{
    unsigned int i;
    float sum = 0.0;

    for (i = 0; i < size; i++) {
        sum += src[i];
    }

    *result = sum / size;
}

#elif __arm__

#include "arm_math.h"

void mean_f32(float *src, unsigned int size, float *result)
{
    arm_mean_f32(src, size, result);
}

#endif
