#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char filename[FILENAME_MAX];
        unsigned int freq;
        unsigned int sample_freq;
        unsigned int number;
    } param;

    double *sample;
};

#endif