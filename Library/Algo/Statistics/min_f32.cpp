#ifdef _WIN32

#include <math.h>
#include <float.h>

void min_f32(float *src, unsigned int size, float *result, unsigned int *index)
{
    unsigned int i;
    float min = FLT_MAX;
    unsigned int min_index;

    for (i = 0; i < size; i++) {
        if (src[i] < min) {
            min = src[i];
            min_index = i;
        }
    }

    *result = min;
    *index = min_index;
}

#elif __arm__

#include "arm_math.h"

void min_f32(float *src, unsigned int size, float *result, unsigned int *index)
{
    arm_min_f32(src, size, result, index);
}

#endif
