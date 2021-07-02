#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int i;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_content = file_load(app.param.input_filename, &app.input_file_size);
    if (NULL == app.input_file_content) {
        info("file not found\n");
        return -1;
    }

    app.output_file_size = app.param.width * app.param.height;
    app.output_file_content = (unsigned char *)malloc(app.output_file_size);
    
    for (i = 0; i < app.param.height; i++) {
        memcpy(&app.output_file_content[app.param.width * i],
            &app.input_file_content[app.param.stride * i],
            app.param.width);
    }

    file_save(app.param.output_filename, app.output_file_content, app.output_file_size);

    return 0;
}
