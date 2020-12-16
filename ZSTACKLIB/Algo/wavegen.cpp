#include <math.h>
#include <stdio.h>

#define PI 3.1415927
#define DOUBLE_PI   6.283185307179586476925286766559

int wavegen(float *output, int length, int freq, int sample_freq)
{
	int i;
	double value;

	if (NULL == output)
		return -1;

	for (i = 0; i < length; i++) {
		value = sin(freq * PI * i * 2 / sample_freq);

		printf("%f \n", value);

		//output[i] = value;
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
