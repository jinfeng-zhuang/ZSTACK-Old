#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int i, j;
    u64 in_size;
    u32 frame_size;
    u32 frame_num;
    u8 *buffer = NULL;
    u32 ret;
    u32 new_frame_size;
    u8 *new_frame_buffer = NULL;
    u8 *tmpsrc           = NULL;
    u8 *tmpdst           = NULL;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    in_size = file_size(app.param.input_filename);
    if (-1 == in_size) {
        printf("%s not found\n", app.param.input_filename);
    }

    frame_size = app.param.width * app.param.height * 3 / 2;
    frame_num  = in_size / frame_size;

    new_frame_size = app.param.stride * app.param.height * 3 / 2;

    // 10 second
    if (frame_num > 10*30) {
        frame_num = 10 * 30;
        printf("limit frame number to %d\n", frame_num);
    } else {
        printf("frame number is %d\n", frame_num);
    }

    buffer = (u8 *)malloc(frame_num * frame_size);
    if (NULL == buffer) {
        printf("malloc failed for frames\n");
        goto END;
    }

    new_frame_buffer = (u8 *)malloc(new_frame_size);
    if (NULL == new_frame_buffer) {
        goto END;
    }

    ret = file_load(app.param.input_filename, 0, buffer, frame_num * frame_size);
    if (ret != frame_num * frame_size) {
        printf("read failed, require %d but read back %d bytes\n",
               frame_num * frame_size, ret);
        goto END;
    }

    for (i = 0; i < frame_num; i++) {
        // Y
        tmpdst = new_frame_buffer;
        tmpsrc = buffer;
        for (j = 0; j < app.param.height; j++) {
            memcpy(&tmpdst[j * app.param.stride], &tmpsrc[j * app.param.width], app.param.width);
        }

        // U
        tmpdst = &new_frame_buffer[app.param.stride * app.param.height];
        tmpsrc = &buffer[app.param.width * app.param.height];
        for (j = 0; j < app.param.height / 2; j++) {
            memcpy(&tmpdst[(app.param.stride / 2) * j], &tmpsrc[(app.param.width / 2) * j], app.param.width / 2);
        }

        // V
        tmpdst = &new_frame_buffer[app.param.stride * app.param.height * 5 / 4];
        tmpsrc = &buffer[app.param.width * app.param.height * 5 / 4];
        for (j = 0; j < app.param.height / 2; j++) {
            memcpy(&tmpdst[(app.param.stride / 2) * j], &tmpsrc[(app.param.width / 2) * j], app.param.width / 2);
        }

        file_append(app.param.output_filename, new_frame_buffer, new_frame_size);
    }

END:
    if (buffer)
        free(buffer);
    if (new_frame_buffer)
        free(new_frame_buffer);

    return 0;
}
