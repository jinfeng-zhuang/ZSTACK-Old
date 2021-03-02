#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <zstack.h>

#include "hs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char input_filename[FILENAME_MAX];
        char output_filename[FILENAME_MAX];
    } param;

    unsigned char *input_file_content;
    unsigned int   input_file_size;

    unsigned char *output_file_content;
    unsigned int   output_file_size;

    struct list_head card_queue;
    struct list_head event_queue;
};

extern void hs_card_dump(struct list_head *head);

#ifdef __cplusplus
}
#endif

#endif
