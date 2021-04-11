#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    MSG msg;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    window_class_register();

    HWND hwnd = CreateWindowEx(
            0,
            TEXT("FieldMergeWindow"),
            TEXT("FieldMergeWindow"),
            WS_OVERLAPPEDWINDOW,
            0, 0, 1080, 720,
            NULL,
            NULL,
            NULL,
            NULL);

    ShowWindow(hwnd, 1);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    return 0;
}
