#define ENABLE_LOG

#include <Windows.h>
#include <process.h>

#include <stdio.h>

#include <zstack/log.h>

extern int YUVWindow_Register(HINSTANCE hInstance);

HWND YUVWindowHwnd = NULL;

unsigned int __stdcall worker_thread(void *arg)
{
	MSG msg;
    HWND hwnd;
    HANDLE hThread;
    HINSTANCE hInstance = NULL;
    HINSTANCE hPrevInstance = NULL;
    char lpCmdLine = NULL;
    int nShowCmd = 10;

    YUVWindow_Register(hInstance);

    hwnd = CreateWindowEx(
        0,
        TEXT("YUVWindow"),
        TEXT("Demo"),
        WS_OVERLAPPEDWINDOW,
        0, 0, 720, 576,
        NULL,
        NULL,
        hInstance,
        NULL);

    YUVWindowHwnd = hwnd;

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    return (int)(msg.wParam);
}

void YUVWindow_Init(void)
{
    HANDLE hThread;

    hThread = (HANDLE)_beginthreadex(NULL, 0, worker_thread, NULL, 0, NULL);
    if (NULL == hThread)
        return;

    while (NULL == YUVWindowHwnd)
        Sleep(100);
}

void YUVWindow_Set(struct YUVWindowStruct *r)
{
    if (YUVWindowHwnd)
        SendMessage(YUVWindowHwnd, WM_USER, (WPARAM)r, NULL);
    else
        log(LOG_WARNING, "Window not inited\n");
}
