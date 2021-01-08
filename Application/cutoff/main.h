#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char filename[FILENAME_MAX];
        char output_filename[FILENAME_MAX];
        unsigned int start;
        unsigned int end;
        unsigned int size;
    } param;

    unsigned char *file_content;
    unsigned int file_size;

    unsigned int output_file_size;
};

#endif