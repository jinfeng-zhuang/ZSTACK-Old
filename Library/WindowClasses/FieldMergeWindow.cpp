#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>

#include <zstack/ringbuf.h>
#include <zstack/log.h>
#include <zstack/yuv.h>
#include <zstack/platform.h>

extern int YUVWindow_Register(HINSTANCE hInstance);

struct {
    HWND edit_fieldA;
    HWND edit_fieldB;
    CHAR content_fieldA[32];
    CHAR content_fieldB[32];
    HWND button_prev;
    HWND button_next;
    HWND yuv;
    HWND match;
} window_ctrl;

struct db {
    int top;
    int bottom;
    int match;
    unsigned char *merged_buffer;
};

static unsigned int worker = 0;

// TODO
// should be here, otherwise it will be overwritten by stack, which is using in worker thread
static struct db db;
static struct YUVWindowStruct r;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hPaper;
    static HDC hDeck;
    static HBRUSH hBrush;
    static struct ring ringbuffer = {0, 0, 0, 0};
    struct YUVWindowStruct yuv_struct;
    
    HDC hdcWindow;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int do_update = 0;
    HWND hwnd;

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    //TRACE;

    switch (uMsg)
    {
    case WM_CREATE:

        YUVWindow_Register(hInstance);

        //--------------------------------------------------------
        window_ctrl.edit_fieldA = CreateWindowEx(
            0,
            TEXT("EDIT"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            0, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        window_ctrl.edit_fieldB = CreateWindowEx(
            0,
            TEXT("EDIT"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            50, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        window_ctrl.button_prev = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("<<"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            100, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        window_ctrl.button_next = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT(">>"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            150, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        window_ctrl.match = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Match"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            250, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        window_ctrl.yuv = CreateWindowEx(
            0,
            TEXT("YUVWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            0, 30, 720, 576,
            hWnd,
            NULL,
            hInstance,
            NULL);

        break;
    case WM_SIZE:
        //GetClientRect(hWnd, &rect);

        break;

    case WM_PAINT:
        hdcWindow = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
        break;

    case WM_COMMAND:
        GetWindowTextA(window_ctrl.edit_fieldA, window_ctrl.content_fieldA, sizeof(window_ctrl.content_fieldA));
        GetWindowTextA(window_ctrl.edit_fieldB, window_ctrl.content_fieldB, sizeof(window_ctrl.content_fieldB));

        db.top = atoi(window_ctrl.content_fieldA);
        db.bottom = atoi(window_ctrl.content_fieldB);
        db.match = 0;

        do_update = 1;

        if ((HWND)lParam == window_ctrl.button_prev) {
            if (db.bottom > 0)
                db.bottom--;
            _snprintf(window_ctrl.content_fieldB, sizeof(window_ctrl.content_fieldB), "%d", db.bottom);
            SetWindowTextA(window_ctrl.edit_fieldB, window_ctrl.content_fieldB);

            do_update = 1;
        }
        else if ((HWND)lParam == window_ctrl.button_next) {
            db.bottom++;
            _snprintf(window_ctrl.content_fieldB, sizeof(window_ctrl.content_fieldB), "%d", db.bottom);
            SetWindowTextA(window_ctrl.edit_fieldB, window_ctrl.content_fieldB);

            do_update = 1;
        }
        else if ((HWND)lParam == window_ctrl.match) {
            db.top++;
            _snprintf(window_ctrl.content_fieldA, sizeof(window_ctrl.content_fieldA), "%d", db.top);
            SetWindowTextA(window_ctrl.edit_fieldA, window_ctrl.content_fieldA);
            db.match = 1;
        }

        if (do_update && worker) {
            PostThreadMessage(worker, WM_USER, MSG_SET_DATA, (WPARAM)&db);
        }
        break;

    case WM_USER:
        if (wParam == MSG_SET_HANDLE) {
            worker = lParam;
        }
        else if (wParam == MSG_SET_DATA) {
            memcpy(&r, (void *)lParam, sizeof(struct YUVWindowStruct));
            PostMessage(window_ctrl.yuv, WM_USER, (WPARAM)&r, NULL);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int FieldMergeWindow_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("FieldMergeWindow");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

