#ifndef ZSTACK_ALGO_H
#define ZSTACK_ALGO_H

extern void fft(double * data, int n, int isInverse);
extern int wavegen(float *output, int length, int freq, int sample_freq);
extern int wavegen_complex(float *output, int length, int freq, int sample_freq);

#endif
