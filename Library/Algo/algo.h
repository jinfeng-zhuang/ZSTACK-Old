#ifndef ZSTACK_ALGO_H
#define ZSTACK_ALGO_H

extern void fft(float *real,float *image,int N);
extern int wavegen(float *output, int length, int freq, int sample_freq);
extern int wavegen_complex(double *output, int length, int freq, int sample_freq);
extern int hanning_window(float *io, unsigned int size);
extern int linear_regression(float *y, unsigned int length);

extern int power_spectrum_complex(float *complex, float *power, unsigned int size);
extern int power_spectrum(float *real, float *image, float *power, unsigned int size);

#endif
