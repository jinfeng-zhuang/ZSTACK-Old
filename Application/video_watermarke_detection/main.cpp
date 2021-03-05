#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int i;
    unsigned char *buffer;
    uint64_t frame_count;
    uint64_t frame_size;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_size = file_size(app.param.input_filename);

    frame_size = app.param.width * app.param.height * 3 / 2;
    frame_count = app.input_file_size / frame_size;

    for (i = 0; i < frame_count; i++) {
        buffer = file_load_ex(app.param.input_filename, (uint64_t)i * frame_size, 16);
        hexdump(buffer, 16);
    }

    return 0;
}
