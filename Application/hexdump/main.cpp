#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_content = file_load(app.param.input_filename, &app.input_file_size);

    hexdump(app.input_file_content, app.input_file_size);

    return 0;
}
