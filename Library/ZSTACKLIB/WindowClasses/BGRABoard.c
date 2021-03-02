#include <Windows.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#define BITMAP_ARRAY_SIZE   (4)
#define FRAMERATE           (30)

static HBITMAP hBitmapMask;
static HBITMAP hBitmapArray[BITMAP_ARRAY_SIZE];

static unsigned char bgra[1920*1080*4];

struct WindowManager {
};

void BGRABoard_Set(unsigned char *buffer, int width, int height)
{
    int realwidth, realheight;
    int line;

    realwidth = (width < 1920) ? width : 1920;
    realheight = (height < 1080) ? height : 1080;

    for (line = 0; line < height; line++) {
        memcpy(&bgra[line * 1920 * 4], &buffer[line * width * 4], realwidth * 4);
    }
}

// custom event supported
// set dispinfo
// max size should <= 2K
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    HDC hdcMem;
    int i;
    BLENDFUNCTION ftn = { 0 };

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:

        hdc = GetDC(hWnd);

        hBitmapMask = CreateCompatibleBitmap(hdc, 1920, 1080);

        for (i = 0; i < BITMAP_ARRAY_SIZE; i++)
            hBitmapArray[i] = CreateCompatibleBitmap(hdc, 1920, 1080);

        ReleaseDC(hWnd, hdc);

        break;
    case WM_SIZE:
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // move to WM_CREATE
        hdcMem = CreateCompatibleDC(hdc);

        SetBitmapBits(hBitmapArray[0], 1920 * 1080 * 4, bgra);

        // select correct image
        SelectObject(hdcMem, hBitmapArray[0]);

        BitBlt(hdc, 0, 0, 1920, 1080, hdcMem, 0, 0, SRCCOPY);

        // move to WM_DESTROY
        DeleteDC(hdcMem);

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_DROPFILES:
        wchar_t filename[100];
        DragQueryFile((HDROP)wParam, 0, filename, 100);
        DragFinish((HDROP)wParam);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int BGRABoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("BGRABoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

