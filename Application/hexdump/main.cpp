#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int count;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_content = file_load(app.param.input_filename, &app.input_file_size);

    count = app.input_file_size;

    if (app.param.count != 0)
        count = app.param.count;

    hexdump(app.input_file_content, count);

    return 0;
}
