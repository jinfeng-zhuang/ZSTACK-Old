#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char input_filename[FILENAME_MAX];
        char output_filename[FILENAME_MAX];
    } param;

    unsigned char *input_file_content;
    unsigned long long input_file_size;

    unsigned char *output_file_content;
    unsigned int   output_file_size;
};

#ifdef __cplusplus
}
#endif

#endif