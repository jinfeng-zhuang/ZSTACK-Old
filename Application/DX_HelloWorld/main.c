#include <zstack/zstack.h>
#include <d3d12.h>
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

    //ret = Direct3DCreate12();

    return 0;
}
