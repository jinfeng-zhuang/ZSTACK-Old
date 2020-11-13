#define ENABLE_LOG
#define LOG_MODULE_NAME "RingBufferWindow"

#include <Windows.h>

#include <zstack/ringbuf.h>
#include <zstack/log.h>
#include <zstack/yuv.h>

static struct YUVWindowStruct data_g;

static void Draw(HDC hdc, int width, int height, HBRUSH brush, struct YUVWindowStruct *data)
{
    struct ring normalize;
    int position[4];
    float scale;
    RECT rect;
    int i, j;
    unsigned int color;
    unsigned char r,g,b;

    if (NULL == data)
        return;

    for (i=0;i<data->width; i++) {
        for (j=0; j <data->height; j++) {
            r = g = b = I420_Y(i, j, data->width, data->height, data->buffer);
            color = r<<0 | g<<8 | b<<16;
            SetPixel(hdc, i, j, color);
        }
    }
}

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
    HBITMAP hPaperOld;
    RECT rect;
    struct ring ringbuf = {0, 100, 30, 50};

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:

        GetClientRect(hWnd, &rect);

        hdcWindow = GetDC(hWnd);
        
        hDeck = CreateCompatibleDC(hdcWindow);
        hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);
        hBrush = CreateSolidBrush(RGB(0x33, 0x99, 0xFF));

        SelectObject(hDeck, hPaper);

        ReleaseDC(hWnd, hdcWindow);

        //SendMessage(hWnd, WM_USER, (WPARAM)&ringbuf, NULL);

        break;
    case WM_SIZE:
        GetClientRect(hWnd, &rect);

        hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);

        hPaperOld = (HBITMAP)SelectObject(hDeck, hPaper);
        
        DeleteObject(hPaperOld);
        break;

    case WM_PAINT:
        hdcWindow = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);

        Draw(hDeck, rect.right - rect.left, rect.bottom - rect.top, hBrush, &data_g);
        
        BitBlt(hdcWindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDeck, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
        break;
    case WM_USER:
        memcpy((unsigned char *)&data_g, (unsigned char *)wParam, sizeof(struct YUVWindowStruct));
        InvalidateRect(hWnd, NULL, FALSE);
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

