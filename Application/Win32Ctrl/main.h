#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <zstack/types.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char input_filename[FILENAME_MAX];
        char output_filename[FILENAME_MAX];
        char classname[FILENAME_MAX];
    } param;

    u8* input_file_content;
    u64 input_file_size;

    u8* output_file_content;
    u64   output_file_size;
};

#ifdef __cplusplus
}
#endif

#endif