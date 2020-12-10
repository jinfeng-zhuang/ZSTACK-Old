#include <zstack.h>
#include <vs.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    unsigned int chip_id;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    log(LOG_DEBUG, "sxmem %s --address=0x%x --bytecount=%d --operation=%s --format=%s --data=\"%s\" --datafile=\"%s\"\n",
        app.param.ip, app.param.address, app.param.bytecount,
        app.param.operation == OPERATION_WRITE ? "write" : "read",
        app.param.format == FORMAT_STRUCT ? "struct" : (app.param.format == FORMAT_BINARY ? "binary" : "hexdump"),
        app.param.data,
        app.param.datafile);

    if (0 != dbg_init(app.param.ip))
        return -1;

    chip_id = vs_chip_id_get();
    if ((chip_id != CHIP_SX7B) && (chip_id != CHIP_SX7A) && (chip_id != CHIP_SX8B) && (chip_id != CHIP_SX8A)) {
        log(LOG_WARNING, "CHIP Not supported\n");
        return -1;
    }

    if ((app.param.operation == OPERATION_READ) && (app.param.format == FORMAT_HEXDUMP)) {
        // TODO while to read all, per 256 bytes
        ret = dbg_host_read8(app.param.address, (unsigned char *)app.param.data, app.param.bytecount);
        if (0 == ret) {
            hexdump(app.param.data, app.param.bytecount);
        }
    }

    if ((app.param.operation == OPERATION_WRITE) && (app.param.format == FORMAT_STRUCT) && (app.param.data[0] != '\0')) {
        ret = mem_format_parser_simple(app.param.data, app.buffer);
        ret = (ret > app.param.bytecount) ? app.param.bytecount : ret;
        hexdump(app.buffer, ret);
        ret = dbg_host_write8(app.param.address, app.buffer, ret);
        if (0 == ret) {
            log(LOG_USER, "Done!\n");
        }
    }

    return 0;
}
