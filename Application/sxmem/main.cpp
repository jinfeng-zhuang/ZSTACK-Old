#include <zstack.h>
#include <vs.h>

#include "main.h"

struct application app;

int mem_format_detect(void)
{
    unsigned int i;
    char *str = app.param.data;

    if ((str[0] == '\0') && (app.param.bytecount > 0) && (app.param.operation == OPERATION_READ))
        return FORMAT_HEXDUMP;

    for (i = 0; i < strlen(str); i++) {
        if ('.' == str[i]) {
            return FORMAT_BINARY;
        }
        else if (':' == str[i]) {
            return FORMAT_STRUCT;
        }
        else {
        }
    }

    return FORMAT_HEXDUMP;
}

int main(int argc, char *argv[])
{
    int ret;
    unsigned int chip_id;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    if (0 != dbg_init(app.param.ip))
        return -1;

    chip_id = vs_chip_id_get();
    if ((chip_id != CHIP_SX7B) && (chip_id != CHIP_SX7A) && (chip_id != CHIP_SX8B) && (chip_id != CHIP_SX8A)) {
        log_warn("CHIP Not supported\n");
        return -1;
    }

    // determine the format
    app.format = mem_format_detect();

    printf("app.format = %d\n", app.format);

    if ((app.param.operation == OPERATION_READ) && (app.format == FORMAT_HEXDUMP)) {
        // TODO while to read all, per 256 bytes
        ret = dbg_host_read8(app.param.address, (unsigned char *)app.param.data, app.param.bytecount);
        if (0 == ret) {
            hexdump(app.param.data, app.param.bytecount);
        }
    }

    if ((app.param.operation == OPERATION_WRITE) && (app.format == FORMAT_STRUCT) && (app.param.data[0] != '\0')) {
        ret = mem_format_parser_simple(app.param.data, app.buffer);
        ret = dbg_host_write8(app.param.address, app.buffer, ret);
        if (0 == ret) {
        }
    }

    return 0;
}
