#ifdef _WIN32

#include <math.h>
#include <float.h>

extern void power_f32(float *src, unsigned int size, float *result);

void rms_f32(float *src, unsigned int size, float *result)
{
    float sum = 0.0;

    power_f32(src, size, &sum);

    *result = sqrt(sum) / size;
}

#elif __arm__

#include "arm_math.h"

void rms_f32(float *src, unsigned int size, float *result)
{
    arm_rms_f32(src, size, result);
}

#endif
