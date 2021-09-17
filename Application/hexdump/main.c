#include <zstack/zstack.h>
#include <stdio.h>
#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    u8* buffer;
    u32 offset;
    u32 bytes_read;
    u32 flags;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    //log_init(app.param.log_config);

    buffer = (u8*)malloc(app.param.page_size);
    if (NULL == buffer)
        return -1;

    flags = 0;
    if (app.param.ascii_flag) {
        flags |= HEXDUMP_ASCII;
    }
    if (app.param.addr_flag) {
        flags |= HEXDUMP_ADDR;
    }

    offset = 0;
    while (1) {
        bytes_read = file_load(app.param.input_filename, offset, buffer, app.param.page_size);
        if (-1 == bytes_read) {
            warn("file %s load failed\n", app.param.input_filename);
            break;
        }
        else if (bytes_read < app.param.page_size) {
            hexdump(buffer, bytes_read, flags);
            break;
        }
        else {
            hexdump(buffer, app.param.page_size, flags);
            offset += app.param.page_size;
        }
        (void)getchar();
    }

    return 0;
}
