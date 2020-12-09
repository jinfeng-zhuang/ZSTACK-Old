#include <Windows.h>
#include <process.h>

#include <stdio.h>

#include <zstack/log.h>
#include <zstack/platform.h>

extern int Platform_Register(HINSTANCE hInstance);

static int (*platform_callback)(int evt, void *in, void *out);

struct db {
    int top;
    int bottom;
    unsigned char *merged_buffer;
};

static unsigned int __stdcall worker_thread(void *arg)
{
    MSG msg;
    unsigned int parent_thread_id;
    int ret;
    static HWND hwnd = 0;
    unsigned int output;

    parent_thread_id = (DWORD)arg;

    // force create the message queue
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

    // notify ready
    PostThreadMessage(parent_thread_id, WM_USER, 0, 0);

    while (1) {
        memset(&msg, 0, sizeof(MSG));

        ret = GetMessage(&msg, NULL, 0, 0);

        if (msg.wParam == MSG_SET_HANDLE) {
            hwnd = (HWND)msg.lParam;
        }
        else if (msg.wParam == MSG_SET_DATA) {
            ret = platform_callback(msg.wParam, (void *)msg.lParam, &output);
            if (ret != 0) {
                log(LOG_WARNING, "Callback return failed\n");
            }
            else {
                if  (hwnd) {
                    PostMessage(hwnd, WM_USER, MSG_SET_DATA, output);
                }
            }
        }
    }

    return 0;
}

int Platform_Init(const char *classname, int (*callback)(int evt, void *in, void *out))
{
    HANDLE hThread;
	MSG msg;
    HWND hwnd;
    HINSTANCE hInstance = NULL;
    HINSTANCE hPrevInstance = NULL;
    char lpCmdLine = NULL;
    int nShowCmd = 10;
    unsigned int worker_thread_id;

    if (NULL == classname) {
        log(LOG_ERROR, "classname shouldn't be NULL\n");
        return -1;
    }

    platform_callback = callback;

    Platform_Register(NULL);

    hThread = (HANDLE)_beginthreadex(NULL, 0, worker_thread, (void *)GetCurrentThreadId(), 0, &worker_thread_id); // CREATE_SUSPENDED, 0 = immediately
    if (NULL == hThread)
        return -1;

    // wait for thread ready
    GetMessage(&msg, NULL, 0, 0);

    hwnd = CreateWindowEx(
        0,
        classname,
        TEXT("PlatformApp"),
        WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (NULL == hwnd) {
        log(LOG_ERROR, "Window '%s' create failed\n", classname);
        return -1;
    }

    // tell the worker thread id
    PostMessage(hwnd, WM_USER, MSG_SET_HANDLE, worker_thread_id);

    // tell the hwnd
    PostThreadMessage(worker_thread_id, WM_USER, MSG_SET_HANDLE, (LPARAM)hwnd);

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    return 0;
}
