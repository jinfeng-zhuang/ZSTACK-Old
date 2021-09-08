#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <zstack/types.h>
#include <zstack/yuv.h>

#define LOG_CONFIG_LENGTH   (256)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char input_filename[FILENAME_MAX];
        char output_filename[FILENAME_MAX];
        int  width;
        int  height;
        enum YUVFormat iformat;
        enum YUVFormat oformat;
    } param;
    
    FILE* ifp;
    u8* iframe;
    u32 iframe_size;

    u8* oframe;
    u32 oframe_size;

    u8* o_y;
    u8* o_u;
    u8* o_v;
    u8* o_uv;
};

#ifdef __cplusplus
}
#endif

#endif