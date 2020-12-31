#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char filename[FILENAME_MAX];
        int flag_power;
    } param;

    float *sample;
    float *complex;
    float *r_data;
    float *i_data;
    float *A_data;
};

#endif