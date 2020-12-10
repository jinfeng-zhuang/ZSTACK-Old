#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>

#include <3rdparty/getopt.h>

#include <zstack.h>

struct application {
    char *debug;

    int width;
    int height;
    int top_field_first;
    int flag_window;
    char top_filename[128];
    char bottom_filename[128];
    char output_filename[128];
    int top_index;
    int bottom_index;
    int func;
    FILE *top_fd;
    FILE *bottom_fd;
    FILE *output_fd;
    unsigned char *top_buffer;
    unsigned char *bottom_buffer;
    unsigned char *output_buffer;

    int *match_list;
};

#endif
