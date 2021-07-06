#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    return 0;
}
