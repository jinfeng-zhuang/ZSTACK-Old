#define _USE_MATH_DEFINES

#include <math.h>

int hanning_window(double *io, unsigned int size)
{
    unsigned int i;
    double temp;

    for (i = 0; i < size; i++) {
        temp = 0.5 * (1.0 + cos(2.0 * M_PI * i / (size-1) - M_PI));
        io[i] *= temp;
    }

    return 0;
}
