#include "arm_math.h"
#include "arm_const_structs.h"
#include <stdio.h>

extern float data[];

#define USE_50HZ
#define USE_100HZ
#define USE_200HZ
#define USE_2500HZ
#define USE_2550HZ

#define Fs          (15360)
#define FFT_SIZE    (4096)

float fft_in_out_buffer[FFT_SIZE * 2]; // INPUT[4096], OUTPUT[4096]
float fft_mag_buffer[FFT_SIZE];

static void wavegen_complex(void)
{
    unsigned int i;
    
    for (i = 0; i < FFT_SIZE; i++) {
        fft_in_out_buffer[2 * i + 0] = 
            0   +
#ifdef USE_50HZ
            100*arm_sin_f32(2.0f*PI*i*50.0f/Fs) +
#endif

#ifdef USE_100HZ
            200*arm_sin_f32(2.0f*PI*i*100.0f/Fs) +
#endif

#ifdef USE_200HZ
            300*arm_sin_f32(2.0f*PI*i*200.0f/Fs) +
#endif

#ifdef USE_2500HZ
            200*arm_sin_f32(2.0f*PI*i*2500.0f/Fs)  +
#endif

#ifdef USE_2550HZ
            300*arm_sin_f32(2.0f*PI*i*2550.0f/Fs)+
#endif
            0;
        fft_in_out_buffer[2 * i + 1] = 0;
    }
}

static void wavegen_real(void)
{
    unsigned int i;
    
    for (i = 0; i < FFT_SIZE; i++) {
        fft_in_out_buffer[i] = 
            0   +
#ifdef USE_50HZ
            100*arm_sin_f32(2.0f*PI*i*50.0f/Fs) +
#endif

#ifdef USE_100HZ
            200*arm_sin_f32(2.0f*PI*i*100.0f/Fs) +
#endif

#ifdef USE_200HZ
            300*arm_sin_f32(2.0f*PI*i*200.0f/Fs) +
#endif

#ifdef USE_2500HZ
            200*arm_sin_f32(2.0f*PI*i*2500.0f/Fs)  +
#endif

#ifdef USE_2550HZ
            300*arm_sin_f32(2.0f*PI*i*2550.0f/Fs)+
#endif
            0;
    }
}

void test_fft(void)
{
    unsigned int i;

    //wavegen_complex();
    for (i = 0; i < FFT_SIZE; i++) {
        fft_in_out_buffer[i * 2 + 0] = data[i];
        fft_in_out_buffer[i * 2 + 1] = 0;
    }

    arm_cfft_f32(&arm_cfft_sR_f32_len4096, fft_in_out_buffer, 0, 1);

    for (i = 0; i < FFT_SIZE; i++) {
        printf("%f %f\n", fft_in_out_buffer[i * 2 + 0], fft_in_out_buffer[i * 2 + 1]);
    }
}

void test_rfft(void)
{
    unsigned int i;
    arm_rfft_fast_instance_f32 S;

    //wavegen_real();
    
    arm_rfft_fast_init_f32(&S, 4096);
    arm_rfft_fast_f32(&S, data, fft_mag_buffer, 0);
    //arm_cmplx_mag_f32(fft_mag_buffer, fft_mag_buffer, FFT_SIZE);

    for (i = 0; i < FFT_SIZE; i++) {
        printf("%f\n", fft_mag_buffer[i]);
    }
}
