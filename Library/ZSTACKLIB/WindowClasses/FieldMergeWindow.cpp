#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>

#include <zstack.h>

extern int YUVWindow_Register(HINSTANCE hInstance);

static struct {
    HWND hwnd_yuv;
    HWND hwnd_yuv_top;
    HWND hwnd_yuv_bottom;

    // data
    int width;
    int height;
    unsigned char *data;

    RECT top;
    RECT bottom;
    RECT result;
} database; // TODO: not in stack

static void layout_set(RECT *client)
{
    int width = client->right - client->left;
    int height = client->bottom - client->top;
    int margin = 10;

    database.top.left = 0;
    database.top.top = 0;
    database.top.right = width / 2;
    database.top.bottom = height / 2;

    database.bottom.left = 0;
    database.bottom.top = height / 2;
    database.bottom.right = width / 2;
    database.bottom.bottom = height;

    database.result.left = width / 2;
    database.result.top = 0;
    database.result.right = width;
    database.result.bottom = height;

    // Add Padding
    database.top.left += margin;
    database.top.top += margin;
    database.top.right -= margin;
    database.top.bottom -= margin;

    database.bottom.left += margin;
    database.bottom.top += margin;
    database.bottom.right -= margin;
    database.bottom.bottom -= margin;

    database.result.left += margin;
    database.result.top += margin;
    database.result.right -= margin;
    database.result.bottom -= margin;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hPaper;
    static HDC hDeck;
    static HBRUSH hBrush;
    HINSTANCE hInstance;
    int do_update = 0;
    YUV_INFO info_top;
    YUV_INFO info_bottom;
    int i;
    YUV_INFO info_merge;
    RECT rect;

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    //TRACE;

    switch (uMsg)
    {
    case WM_CREATE:

        YUVWindow_Register(hInstance);

        //--------------------------------------------------------
        GetClientRect(hWnd, &rect);
        layout_set(&rect);

        database.hwnd_yuv = CreateWindowEx(
            0,
            TEXT("YUVWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD | YUV_STYLE_EXTERNAL,
            database.result.left,
            database.result.top,
            database.result.right - database.result.left,
            database.result.bottom - database.result.top,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.hwnd_yuv_top = CreateWindowEx(
            WS_EX_TOOLWINDOW,
            TEXT("YUVWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            database.top.left,
            database.top.top,
            database.top.right - database.top.left,
            database.top.bottom - database.top.top,
            hWnd,
            NULL,
            hInstance,
            NULL);

        database.hwnd_yuv_bottom = CreateWindowEx(
            0,
            TEXT("YUVWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            database.bottom.left,
            database.bottom.top,
            database.bottom.right - database.bottom.left,
            database.bottom.bottom - database.bottom.top,
            hWnd,
            NULL,
            hInstance,
            NULL);
        break;
    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        layout_set(&rect);

        MoveWindow(database.hwnd_yuv_top,
            database.top.left,
            database.top.top,
            database.top.right - database.top.left,
            database.top.bottom - database.top.top,
            TRUE);

        MoveWindow(database.hwnd_yuv_bottom,
            database.bottom.left,
            database.bottom.top,
            database.bottom.right - database.bottom.left,
            database.bottom.bottom - database.bottom.top,
            TRUE);

        MoveWindow(database.hwnd_yuv,
            database.result.left,
            database.result.top,
            database.result.right - database.result.left,
            database.result.bottom - database.result.top,
            TRUE);
        break;
    case WM_COMMAND:
        break;
    case WM_USER:
        SendMessage(database.hwnd_yuv_top, WM_USER, YUV_GET_INFO, (LPARAM)&info_top);
        SendMessage(database.hwnd_yuv_bottom, WM_USER, YUV_GET_INFO, (LPARAM)&info_bottom);

        database.width = info_top.width > info_bottom.width ? info_top.width : info_bottom.width;
        database.height = info_top.height > info_bottom.height ? info_top.height : info_bottom.height;
        database.height *= 2;

        if (database.data)
            free(database.data);

        database.data = (unsigned char *)malloc(database.width * database.height); //TODO
        if (NULL == database.data) {
            break;
        }

        for (i = 0; i < database.height / 2; i++) {
            if (info_top.data) {
                memcpy(&database.data[database.width * (i * 2 + 0)], &info_top.data[info_top.width * i], info_top.width);
                //memset(&database.data[database.width * (i * 2 + 0)], 0xFF, info_top.width);
            }
            if (info_bottom.data) {
                memcpy(&database.data[database.width * (i * 2 + 1)], &info_bottom.data[info_bottom.width * i], info_bottom.width);
                //memset(&database.data[database.width * (i * 2 + 1)], 0x00, info_bottom.width);
            }
        }

        info_merge.width = database.width;
        info_merge.height = database.height;
        info_merge.data = database.data;

        SendMessage(database.hwnd_yuv, WM_USER, YUV_UPDATE, (LPARAM)&info_merge);

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
