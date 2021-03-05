#include <zstack.h>
#include <vs.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    unsigned char *buffer;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    dbg_init(app.param.ip);

    buffer = (unsigned char *)malloc(app.param.size);
    if (NULL == buffer)
        return -1;

    ret = vs_pman_enable((sx_chip)vs_chip_id_get(), PMAN_SECURITY_GROUP_ARM, app.param.addr);
    if (0 != ret) {
        warn("pman set failed\n");
        return -1;
    }

    ret = dbg_host_read8(app.param.addr, buffer, app.param.size);
    if (0 == ret) {
        file_save(app.param.output_filename, buffer, app.param.size);
        info("%s", app.param.output_filename);
        free(buffer);
    }
    else {
        warn("dbg_host_read8 failed\n");
    }

    return 0;
}
