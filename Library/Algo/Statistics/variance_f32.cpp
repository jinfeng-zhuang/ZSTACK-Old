#ifdef _WIN32

#include <math.h>
#include <float.h>

extern void sum_f32(float *src, unsigned int size, float *result);
extern void power_f32(float *src, unsigned int size, float *result);

void variance_f32(float *src, unsigned int size, float *result)
{
    float sumOfSquare;
    float sum;

    power_f32(src, size, &sumOfSquare);
    sum_f32(src, size, &sum);

    *result = (sumOfSquare - sum *sum / size) / (size - 1);
}

#elif __arm__

#include "arm_math.h"

void variance_f32(float *src, unsigned int size, float *result)
{
    arm_var_f32(src, size, result, index);
}

#endif
