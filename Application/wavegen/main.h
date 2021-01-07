#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define LOG_CONFIG_LENGTH   (256)

enum {
    FORMAT_DATA,
    FORMAT_CARRAY,
};

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char filename[FILENAME_MAX];
        unsigned int freq;
        unsigned int sample;
        unsigned int number;
        unsigned int format;
    } param;

    float *sample;
};

#endif