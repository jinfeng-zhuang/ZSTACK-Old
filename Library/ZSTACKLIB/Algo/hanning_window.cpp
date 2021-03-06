#define _USE_MATH_DEFINES

#include <math.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

int hanning_window(float *io, unsigned int size)
{
    unsigned int i;
    double temp;

    for (i = 0; i < size; i++) {
        temp = 0.5 * (1.0 + cos(2.0 * M_PI * i / (size-1) - M_PI));
        io[i] *= (float)temp;
    }

    return 0;
}
