#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    param_parser(argc, argv, &app);

    log_init(NULL);

    return 0;
}
