#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    u32 bytes_read;
    u8* buffer = (u8*)malloc(SIZE_1MB);

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    bytes_read = file_load(app.param.input_filename, 0, buffer, SIZE_1MB);
    if (bytes_read != SIZE_1MB) {
        DEBUG("load %s failed\n", app.param.input_filename);
        goto END;
    }

    decode_one_frame(buffer, SIZE_1MB);

END:
    if (buffer)
        free(buffer);

    return 0;
}
