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

    app.input_file_size = 0;
    app.input_file_content = file_load(app.param.input_filename, (unsigned int *)&app.input_file_size);
    if (NULL == app.input_file_content)
        return -1;

    int split_offset = 0;
    char digit_str[16];

    for (i = 0; i < 9557; i++) {
        memset(digit_str, 0, sizeof(digit_str));
        split(digit_str, sizeof(digit_str), (char*)app.input_file_content, app.input_file_size, ',', &split_offset);
        app.sample[i] = atof(digit_str);
    }

    file_save(app.param.output_filename, (unsigned char *)app.sample, sizeof(app.sample));

    return 0;
}
