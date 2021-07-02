#include <zstack.h>
#include <vs.h>

#include "main.h"

extern int avmips_reset_decoder(int channel);

struct application app;

int main(int argc, char *argv[])
{
    int ret;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    ret = dbg_init(app.param.ip);
    if (0 != ret) {
        return -1;
    }

    avmips_reset_decoder(0);

    return 0;
}
