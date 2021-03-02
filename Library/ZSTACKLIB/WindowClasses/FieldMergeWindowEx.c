#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <zstack.h>

enum {
    WIN_TOP,
    WIN_BOTTOM,
    WIN_FRAME,
    WIN_MAX
};

static struct {
    HINSTANCE hInstance;
    HWND hWindow[WIN_MAX];
    RECT rectWindow[WIN_MAX];
} database;

static void resize(int width, int height)
{
    int i;

    for (i=0; i<WIN_MAX; i++) {
        MoveWindow(
            database.hWindow[i],
            database.rectWindow[i].left, database.rectWindow[i].top,
            database.rectWindow[i].right - database.rectWindow[i].left,
            database.rectWindow[i].bottom - database.rectWindow[i].top,
            TRUE);
    }
}

static void create(HINSTANCE hInstance, HWND hWnd)
{
    int i;
    for (i=0; i<WIN_MAX; i++) {
        database.hWindow[i] = CreateWindowEx(
            0,
            TEXT("FieldMergeWindow"),
            NULL,
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            0, 0, 0, 0,
            hWnd,
            NULL,
            hInstance,
            NULL);
    }
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdcWindow;
    static HBITMAP hPaper;
    static HDC hDeck;
    static HBRUSH hBrush;
    RECT rect;
    PAINTSTRUCT ps;

    switch (uMsg)
    {
    case WM_CREATE:
        //create(hWnd);
        break;
    case WM_SIZE:
        //GetClientRect(hWnd, &rect);
        //resize(rect.right - rect.left, rect.bottom - rect.top);
        break;
    case WM_PAINT:
        hdcWindow = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int FieldMergeWindowEx_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("FieldMergeWindowEx");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    database.hInstance = hInstance;

    //YUVWindow_Register(hInstance);

    return 0;
}
