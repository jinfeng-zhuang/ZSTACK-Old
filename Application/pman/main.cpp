#include <zstack.h>
#include <vs.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int ret;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    dbg_init(app.param.ip);

    ret = vs_pman_enable((sx_chip)vs_chip_id_get(), (enum sx_security_group)app.param.security_group, app.param.address);
    if (0 != ret) {
        warn("pman set failed\n");
        return -1;
    }

    return 0;
}
