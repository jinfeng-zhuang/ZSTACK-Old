#define ZSTACK_ENABLE_WINDOWS

#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    unsigned int field_size;
    unsigned int top_offset;
    unsigned int bottom_offset;
    unsigned int offset_t, offset_b;
    int width, height;
    int i;
    struct YUVWindowData data;
    MSG msg;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    width = app.param.width;
    height = app.param.height;

    field_size = width * height * 3 / 2;

    top_offset = field_size * app.param.top_index;
    bottom_offset = field_size * app.param.bottom_index;

    app.top_file_content = file_load_ex(app.param.top_filename, field_size, top_offset);
    app.bottom_file_content = file_load_ex(app.param.bottom_filename, field_size, bottom_offset);

    app.output_file_content = (unsigned char *)malloc(field_size);
    app.output_file_size = field_size;

    if ((app.top_file_content == NULL) || (NULL == app.bottom_file_content) || (NULL == app.output_file_content))
        return -1;

    if (app.param.top_field_first) {
        offset_t = 0;
        offset_b = width;
    }
    else {
        offset_t = width;
        offset_b = 0;
    }

    for (i = 0; i < app.param.height; i++) {
        memcpy(&app.output_file_content[width * i * 2 + offset_t], &app.top_file_content[width * i], width);
        memcpy(&app.output_file_content[width * i * 2 + offset_b], &app.bottom_file_content[width * i], width);
    }

    file_save(app.param.output_filename, app.output_file_content, app.output_file_size);

    if (app.param.flag_window) {
        window_class_register();
        HWND hwnd = CreateWindowEx(
                0,
                TEXT("YUVWindow"),
                TEXT("fieldmerge result"),
                WS_OVERLAPPEDWINDOW,
                0, 0, 800, 600,
                NULL,
                NULL,
                NULL,
                NULL);

        data.width = width;
        data.height = height;
        data.stride = width;
        data.format = YUV_I420;
        data.data = app.output_file_content;

        SendMessage(hwnd, WM_USER, (WPARAM)&data, NULL);

        ShowWindow(hwnd, 1);
        UpdateWindow(hwnd);

	    while (GetMessage(&msg, NULL, 0, 0)) {
		    TranslateMessage(&msg);
		    DispatchMessage(&msg);
	}

    }

    return 0;
}
