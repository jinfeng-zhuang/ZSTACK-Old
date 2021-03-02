#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int i;
    int pixel;
    int frame_size;
    int frame_count;
    int y_size;
    int64_t file_size;
    unsigned char *buffer;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    file_size = fsize(app.param.input_filename);
    if (-1 == file_size) {
        log_err("file open failed: %s\n", app.param.input_filename);
        return -1;
    }

    frame_size = app.param.width * app.param.height * 3 / 2;
    y_size = app.param.width * app.param.height;
    frame_count = file_size / frame_size;

    info("frame count %d\n", frame_count);

    for (i = 0; i < frame_count; i++) {
        buffer = file_load_ex(app.param.input_filename, i * frame_size, frame_size);
        if (NULL == buffer)
            return -1;

        for (pixel = 0; pixel < y_size; pixel++) {
            buffer[pixel] = app.param.value;
        }

        file_append(app.param.output_filename, buffer, frame_size);

        free(buffer);
    }

    return 0;
}
