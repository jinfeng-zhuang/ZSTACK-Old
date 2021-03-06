#include <stdio.h>

int linear_regression(float *y, unsigned int length)
{
    unsigned int i;
    double avg_XY = 0, avg_X = 0, avg_Y = 0, avg_XX = 0, slope, intercept;

    for (i = 0; i < length; i++) {
        avg_XY += i * y[i] / length;
        avg_X += 1.0 * i / length;
        avg_Y += y[i] / length;
        avg_XX += 1.0 * i * i / length;
    }

    slope = (avg_XY - avg_X * avg_Y) / (avg_XX - avg_X * avg_X);
    intercept = avg_Y - slope * avg_X;

    //printf("slope = %f, intercept = %f\n", slope, intercept);

    for (i = 0; i < length; i++) {
        y[i] = y[i] - (float)(slope * i + intercept);
    }

    return 0;
}
