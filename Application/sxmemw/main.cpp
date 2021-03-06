#include <zstack.h>
#include <vs.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    unsigned int size_to_write;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    dbg_init(app.param.ip);

    app.input_file_content = file_load(app.param.input_filename, &app.input_file_size);
    if (NULL == app.input_file_content) {
        warn("open %s failed\n", app.param.input_filename);
        return -1;
    }

    if (app.param.pman_pass) {
        ret = vs_pman_enable((sx_chip)vs_chip_id_get(), PMAN_SECURITY_GROUP_ARM, app.param.addr);
        if (0 != ret) {
            warn("pman set failed\n");
            return -1;
        }
    }

    if (app.param.size)
        size_to_write = app.param.size;
    else
        size_to_write = app.input_file_size;

    ret = dbg_host_write8(app.param.addr, app.input_file_content, size_to_write);
    if (0 == ret) {
        free(app.input_file_content);
    }
    else {
        warn("dbg_host_write8 failed\n");
    }

    return 0;
}
