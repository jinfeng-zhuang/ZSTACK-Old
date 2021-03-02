#ifdef _WIN32

#include <math.h>
#include <float.h>

void dot_product_f32(float *srcA, float *srcB, unsigned int size, float *result)
{
    unsigned int i;
    float sum = 0.0;

    for (i = 0; i < size; i++) {
        sum += srcA[i] * srcB[i];
    }

    *result = sum;
}

#elif __arm__

#include "arm_math.h"

void dot_product_f32(float *srcA, float *srcB, unsigned int size, float *result)
{
    arm_dot_prod_f32(srcA, srcB, size, result);
}

#endif
