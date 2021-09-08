#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    struct YUV_Wrapper* wrapper;
    int i, j;
    unsigned char Y, U, V;
    unsigned int bgr;
    unsigned char *rgb_buffer;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    wrapper = YUV_Wrapper_Init(app.param.format);
    if (NULL == wrapper)
        return -1;

    app.input_file_size = wrapper->FrameSize(app.param.width, app.param.height);
    app.input_file_content = malloc(app.input_file_size);
    app.output_file_size = app.param.width * app.param.height * 3;
    app.output_file_content = malloc(app.output_file_size);

    if ((NULL == app.input_file_content) || (NULL == app.output_file_content))
        return -1;

    rgb_buffer = app.output_file_content;

    file_load(app.param.input_filename, 0, app.input_file_content, wrapper->FrameSize(app.param.width, app.param.height));

    for (j = 0; j < app.param.height; j++) {
        for (i = 0; i < app.param.width; i++) {
            Y = wrapper->GetY(app.input_file_content, app.param.width, app.param.height, i, j);
            U = wrapper->GetU(app.input_file_content, app.param.width, app.param.height, i, j);
            V = wrapper->GetV(app.input_file_content, app.param.width, app.param.height, i, j);

            bgr = yuv2rgba(Y | U << 8 | V << 16);

            rgb_buffer[(j * app.param.width + i) * 3 + 0] = bgr >> 16;
            rgb_buffer[(j * app.param.width + i) * 3 + 1] = bgr >> 8;
            rgb_buffer[(j * app.param.width + i) * 3 + 2] = bgr >> 0;
        }
    }

    file_save(app.param.output_filename, app.output_file_content, app.output_file_size);

    if (app.output_file_content)
        free(app.output_file_content);

    return 0;
}
