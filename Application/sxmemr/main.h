#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char ip[16];
        char input_filename[FILENAME_MAX];
        char output_filename[FILENAME_MAX];
        unsigned int size;
        unsigned int addr;
    } param;

    unsigned char *input_file_content;
    unsigned int   input_file_size;

    unsigned char *output_file_content;
    unsigned int   output_file_size;
};

#endif