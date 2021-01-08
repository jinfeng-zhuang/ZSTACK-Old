#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char filename[FILENAME_MAX];
        char pattern[FILENAME_MAX];
    } param;

    unsigned char *input_file_content;
    unsigned int   input_file_size;

    unsigned char *pattern_content;
    unsigned int   pattern_size;

    unsigned int input_match_offset;
    unsigned int input_match_size;

    unsigned int pattern_match_offset;
    unsigned int pattern_match_size;
};

#endif
