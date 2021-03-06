#define ENABLE_LOG

#include <Windows.h>
#include <process.h>

#include <stdio.h>

#include <zstack/log.h>

extern int RingBufferWindow_Register(HINSTANCE hInstance);

HWND RingBufferWindowHwnd;

unsigned int __stdcall worker_thread(void *arg)
{
	MSG msg;
    HWND hwnd;
    HANDLE hThread;
    HINSTANCE hInstance = NULL;
    HINSTANCE hPrevInstance = NULL;
    char lpCmdLine = NULL;
    int nShowCmd = 10;

    log(LOG_USER, "hInstance = %x hPrevInstance = %x lpCmdLine = %s nShowCmd = %d\n",
        hInstance,
        hPrevInstance,
        lpCmdLine,
        nShowCmd);

    RingBufferWindow_Register(hInstance);

    hwnd = CreateWindowEx(
        0,
        TEXT("RingBufferWindow"),
        TEXT("Demo"),
        WS_OVERLAPPEDWINDOW,
        0, 0, 300, 100,
        NULL,
        NULL,
        hInstance,
        NULL);

    RingBufferWindowHwnd = hwnd;

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    return (int)(msg.wParam);
}

void RingBufferWindow_Init(void)
{
    HANDLE hThread;

    hThread = (HANDLE)_beginthreadex(NULL, 0, worker_thread, NULL, 0, NULL);
    if (NULL == hThread)
        return;
}

void RingBufferWindow_Set(struct ring *r)
{
    SendMessage(RingBufferWindowHwnd, WM_USER, (WPARAM)r, NULL);
}
