#include <zstack/zstack.h>
#include <Windows.h>
#include <commctrl.h>
#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    MSG msg;
    HWND hwnd;
    struct YUV yuvinfo;
    struct YUV_Wrapper *wrapper;
    unsigned char* buffer;
    int frame_size;
    unsigned int bytes_read;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    InitCommonControls();

    Class_YUV_Register(NULL);

    hwnd = CreateWindowEx(
        0,
        TEXT("YUV"),
        TEXT("YUVViewer"),
        WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600,
        NULL,
        NULL,
        NULL,
        NULL);

    wrapper = YUV_Wrapper_Init(app.param.format);
    if (NULL == wrapper)
        goto END;

    frame_size = wrapper->FrameSize(app.param.width, app.param.height);

    buffer = malloc(frame_size);
    if (NULL == buffer)
        goto END;

    bytes_read = file_load(app.param.input_filename, 0, buffer, frame_size);
    if (bytes_read != frame_size) {
        warn("load %s failed\n", app.param.input_filename);
        goto END;
    }

    yuvinfo.format = app.param.format;
    yuvinfo.width = app.param.width;
    yuvinfo.height = app.param.height;
    yuvinfo.buffer = buffer;

    SendMessage(hwnd, WM_USER, 0, &yuvinfo);

    ShowWindow(hwnd, 1);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

END:
    return 0;
}
