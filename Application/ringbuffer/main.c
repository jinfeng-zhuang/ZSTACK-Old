#define ENABLE_LOG

#include <Windows.h>
#include <stdio.h>

#include <zstack/log.h>

extern int RingBufferWindow_Register(HINSTANCE hInstance);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;
    HWND hwnd;
    HANDLE hThread;

    log_init(NULL);

    return 0;

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
        0, 0, 300, 200,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    return (int)(msg.wParam);
}

