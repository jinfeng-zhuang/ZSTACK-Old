#include <Windows.h>
#include <commctrl.h>
#include <zstack/zstack.h>
#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    HWND hwnd;
    MSG msg;
    INITCOMMONCONTROLSEX ICC;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    ICC.dwSize = sizeof(INITCOMMONCONTROLSEX);
    ICC.dwICC = ICC_INTERNET_CLASSES;
    InitCommonControlsEx(&ICC);

    Init_Win32Ctrl_Demo();

    hwnd = CreateWindowEx(
        0,
        app.param.classname,
        TEXT("Win32Ctrl"),
        WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600,
        NULL,
        NULL,
        NULL,
        NULL);

    ShowWindow(hwnd, TRUE);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
