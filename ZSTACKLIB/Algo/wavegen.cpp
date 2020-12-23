#include <math.h>
#include <stdio.h>

#define PI 3.1415927
#define DOUBLE_PI   6.283185307179 //6.283185307179586476925286766559

int wavegen(double *output, int length, int freq, int sample_freq)
{
	int i;
	double value;

	if (NULL == output)
		return -1;

	for (i = 0; i < length; i++) {
		//value = sin(1.0 * freq * PI * i * 2.0 / sample_freq);
        value = sin(1.0 * freq * DOUBLE_PI * i / sample_freq);

		output[i] = value;
	}

	return 0;
}

int wavegen_complex(float *output, int length, int freq, int sample_freq)
{
	int i;
	float value;

	if (NULL == output)
		return -1;

	for (i = 0; i < length; i++) {
		value = sin(freq * i * DOUBLE_PI) / sample_freq;

		output[i * 2 + 0] = value;
		output[i * 2 + 1] = 0;
	}

	return 0;
}
