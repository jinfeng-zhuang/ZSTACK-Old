#include <zstack/zstack.h>
#include <v-silicon/avmips.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    char *buffer;
    unsigned int i;
    unsigned int wr;
    unsigned int rd;
    unsigned int rd_real = 0;

    rd = 0;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    ret = dbg_init(app.param.ip);
    if (0 != ret) {
        return -1;
    }

    //avmips_get_version();

    ret = avmips_log_init(app.param.log_setting);
    if (0 != ret)
        goto END;

    while (1) {
        wr = avmips_get_log_desc();
        if (-1 == wr) {
            goto END;
        }

        avmips_get_log(0);

        if (0 == rd)
            rd = wr;

        if (app.param.window == 0) {
            rd_real = rd;
        }
        else if (wr - rd > app.param.window) {
            rd_real = wr - app.param.window;
        }
        else {
            rd_real = rd;
        }
        
        for (i = rd_real; i < wr; i++) {
            buffer = avmips_get_log(i);
            if (NULL == buffer)
                goto END;

            info("%d/%d) %s", wr - i, wr - rd, buffer);
        }

        rd = wr;
    }

END:
    return 0;
}
