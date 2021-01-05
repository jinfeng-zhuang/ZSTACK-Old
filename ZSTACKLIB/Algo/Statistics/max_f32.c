#ifdef _WIN32

#include <math.h>
#include <float.h>

void max_f32(float *src, unsigned int size, float *result, unsigned int *index)
{
    unsigned int i;
    float max = FLT_MIN;
    unsigned int max_index;

    for (i = 0; i < size; i++) {
        if (src[i] > max) {
            max = src[i];
            max_index = i;
        }
    }

    *result = max;
    *index = max_index;
}

#elif __arm__

#include "arm_math.h"

void max_f32(float *src, unsigned int size, float *result, unsigned int *index)
{
    arm_max_f32(src, size, result, index);
}

#endif
