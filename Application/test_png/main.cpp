#include <zstack.h>
#include <3rdparty/png.h>
#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    int width = 0;
    int height = 0;
    unsigned char *bgra;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    ret = png_to_bgra(app.param.path, &width, &height, &bgra);
    if (ret > 0) {
        printf("PNG[%d] %dx%d\n", ret, width, height);
    }

    return 0;
}
