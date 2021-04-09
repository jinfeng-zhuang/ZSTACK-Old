#include <zstack.h>

#include <Windows.h>

static struct {
    HWND hwnd;
} data;

int main(int argc, char *argv[])
{
    MSG msg;
    char *classname;

    if (argc < 2) {
        printf("test_window_class <classname>\n");
        return 0;
    }

    classname = argv[1];

    log_init(NULL);

    window_class_register();

    data.hwnd = CreateWindowEx(
        0,
        classname,
        TEXT("test_window_class"),
        WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600,
        NULL,
        NULL,
        NULL,
        NULL);

    // NULL means self-test message
    SendMessage(data.hwnd, WM_USER, NULL, NULL);

    ShowWindow(data.hwnd, 1);
    UpdateWindow(data.hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    return 0;
}
