#ifndef ZSTACK_ALGO_H
#define ZSTACK_ALGO_H

extern void fft(float * data, int n, int isInverse);
extern int wavegen(double *output, int length, int freq, int sample_freq);
extern int wavegen_complex(double *output, int length, int freq, int sample_freq);
extern int hanning_window(float *io, unsigned int size);
extern int linear_regression(float *y, unsigned int length);

#endif
