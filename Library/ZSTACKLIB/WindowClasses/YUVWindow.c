// TODO: bmp replace with paper, the effection of SetPixel is too low

#include <Windows.h>

#include <zstack.h>

#include <zstack/list.h>

#include <math.h>

static struct YUVWindowData wParam_demo = {
    400,
    400,
    400,
    YUV_I420,
    NULL
};

struct window_info {
    HWND hwnd;

    HBITMAP hPaper;
    HDC hDeck;
    HBRUSH hBrush;

    struct YUVWindowData data;

    struct list_head list;
};

static struct list_head head;

// 10 bit support
static int Y_Circle(int x, int y, int width, int height)
{
    int cx, cy;
    int r;

    cx = width >> 1;
    cy = height >> 1;

    r = (width > height) ? cy : cx;

    if ((x - cx)*(x - cx) + (y - cy)*(y - cy) <= (r * r))
        return 1;

    return 0;
}

static void init_wParam_demo(void)
{
    int i, j;

    wParam_demo.data = (unsigned char *)malloc(yuv_size(wParam_demo.stride, wParam_demo.height, wParam_demo.format));
    if (NULL == wParam_demo.data) {
        warn("malloc failed\n");
    }
    else {
        for (j = 0; j < wParam_demo.height; j++) {
            for (i = 0; i < wParam_demo.width; i++) {
                wParam_demo.data[j * wParam_demo.stride + i] = Y_Circle(i, j, wParam_demo.width, wParam_demo.height) ? 0 : 0xFF;
            }
        }
    }
}

static struct window_info *search_window_info(HWND hwnd)
{
    struct list_head *pos;
    struct window_info *entry;

    list_for_each(pos, &head) {
        entry = list_entry(pos, struct window_info, list);
        if (entry && (entry->hwnd == hwnd))
            return entry;
    }

    return NULL;
}

static void Draw(HDC hdc, int width, int height, HBRUSH brush, struct YUVWindowData *data)
{
    float scale;
    int i, j;
    unsigned int color;
    unsigned char r,g,b;

    if ((NULL == data) || (data->data == NULL))
        return;

    SelectObject(hdc, brush);

    Rectangle(hdc, 0, 0, width, height);

    for (i = 0; i < data->width; i++) {
        for (j=0; j <data->height; j++) {
            r = g = b = I420_Y(i, j, data->stride, data->height, data->data);
            color = r<<0 | g<<8 | b<<16;
            SetPixel(hdc, i, j, color);
        }
    }
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdcWindow;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    HBITMAP hPaperOld;
    RECT rect;
    unsigned char *buffer = NULL;

    struct window_info *wm = NULL;

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        wm = (struct window_info *)malloc(sizeof(struct window_info));
        if (wm) {
            GetClientRect(hWnd, &rect);

            hdcWindow = GetDC(hWnd);

            wm->hwnd = hWnd;

            wm->hDeck = CreateCompatibleDC(hdcWindow);
            wm->hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);
            wm->hBrush = CreateSolidBrush(RGB(0, 0, 0xFF));

            if (head.next == NULL) {
                INIT_LIST_HEAD(&head);
            }

            list_add_tail(&wm->list, &head);

            SelectObject(wm->hDeck, wm->hPaper);

            ReleaseDC(hWnd, hdcWindow);
        }
        break;
    case WM_SIZE:
        wm = search_window_info(hWnd);
        if (wm) {
            GetClientRect(hWnd, &rect);
            wm->hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);
            hPaperOld = (HBITMAP)SelectObject(wm->hDeck, wm->hPaper); // use new size paper
            DeleteObject(hPaperOld);
        }
        break;

    case WM_PAINT:
        wm = search_window_info(hWnd);
        if (wm) {
            hdcWindow = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &rect);
            Draw(wm->hDeck, rect.right - rect.left, rect.bottom - rect.top, wm->hBrush, &wm->data);
            BitBlt(hdcWindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, wm->hDeck, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_USER:
        wm = search_window_info(hWnd);
        if (wm) {
            if (NULL != wParam) {
                memcpy((unsigned char *)&(wm->data), (unsigned char *)wParam, sizeof(struct YUVWindowData));
            }
            else {
                if (wm->data.data == NULL) {
                    init_wParam_demo();
                }
                memcpy((unsigned char *)&(wm->data), (unsigned char *)&wParam_demo, sizeof(struct YUVWindowData));
            }
            buffer = (unsigned char *)malloc(yuv_size(wm->data.stride, wm->data.height, wm->data.format));
            if (buffer && wm->data.data) {
                memcpy(buffer, wm->data.data, yuv_size(wm->data.stride, wm->data.height, wm->data.format));
                wm->data.data = buffer;
                InvalidateRect(hWnd, NULL, FALSE);
            }
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

int YUVWindow_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("YUVWindow");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

