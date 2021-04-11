#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>

#include <zstack.h>

extern int YUVWindow_Register(HINSTANCE hInstance);

static struct {
    // window handler
    HWND edit_fieldA;
    HWND edit_fieldB;
    CHAR content_fieldA[32];
    CHAR content_fieldB[32];
    HWND button_prev;
    HWND button_next;
    HWND hwnd_yuv;
    HWND hwnd_yuv_top;
    HWND hwnd_yuv_bottom;
    HWND match;
    HWND hwnd_resolution;
    CHAR content_resolution[32];

    // data
    int top;
    int bottom;
    int width;
    int height;
} database;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hPaper;
    static HDC hDeck;
    static HBRUSH hBrush;
    static struct ring ringbuffer = {0, 0, 0, 0};
    HDC hdcWindow;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int do_update = 0;
    int ret;

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    //TRACE;

    switch (uMsg)
    {
    case WM_CREATE:

        YUVWindow_Register(hInstance);

        //--------------------------------------------------------
        database.edit_fieldA = CreateWindowEx(
            0,
            TEXT("EDIT"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            0, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.edit_fieldB = CreateWindowEx(
            0,
            TEXT("EDIT"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            50, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.button_prev = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("<<"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            100, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.button_next = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT(">>"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            150, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.match = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Merge"),
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            250, 0, 50, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.hwnd_resolution = CreateWindowEx(
            0,
            TEXT("EDIT"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            250 + 60, 0, 100, 20,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.hwnd_yuv = CreateWindowEx(
            0,
            TEXT("YUVWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            400, 30, 720, 576,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.hwnd_yuv_top = CreateWindowEx(
            0,
            TEXT("YUVWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            0, 30, 720/2, 576/2,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.hwnd_yuv_bottom = CreateWindowEx(
            0,
            TEXT("YUVWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            0, 30 + 576/2 + 10, 720/2, 576/2,
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
        GetWindowTextA(database.edit_fieldA, database.content_fieldA, sizeof(database.content_fieldA));
        GetWindowTextA(database.edit_fieldB, database.content_fieldB, sizeof(database.content_fieldB));

        database.top = atoi(database.content_fieldA);
        database.bottom = atoi(database.content_fieldB);
        database.match = 0;

        do_update = 1;

        printf("wParam %x lParam %x\n", wParam, lParam);

        if ((HWND)lParam == database.button_prev) {
            if (database.bottom > 0)
                database.bottom--;
            _snprintf(database.content_fieldB, sizeof(database.content_fieldB), "%d", database.bottom);
            SetWindowTextA(database.edit_fieldB, database.content_fieldB);

            do_update = 1;
        }
        else if ((HWND)lParam == database.button_next) {
            database.bottom++;
            _snprintf(database.content_fieldB, sizeof(database.content_fieldB), "%d", database.bottom);
            SetWindowTextA(database.edit_fieldB, database.content_fieldB);

            do_update = 1;
        }
        else if ((HWND)lParam == database.match) {
            // do merge

        }
        else if ((HWND)lParam == database.hwnd_resolution) {
            if (EN_CHANGE == HIWORD(wParam)) {
                GetWindowTextA(database.hwnd_resolution, database.content_resolution, sizeof(database.content_resolution));
                ret = sscanf(database.content_resolution, "%dx%d", &database.width, &database.height);
                if (ret == 2) {
                    SendMessage(database.hwnd_yuv_top, WM_USER, WIN_YUV_RESOLUTION, database.width << 16 | database.height);
                    SendMessage(database.hwnd_yuv_bottom, WM_USER, WIN_YUV_RESOLUTION, database.width << 16 | database.height);
                }
            }
        }
        else {
            printf("wParam = %x lParam = %x\n", wParam, lParam);
        }

        break;

    case WM_USER:
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
