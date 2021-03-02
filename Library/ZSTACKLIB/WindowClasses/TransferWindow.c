/*
 * 用于类似串口和TCP/IP工具的UI
 */

#include <Windows.h>
#include <math.h>
#include <assert.h>
#include <zstack.h>

extern unsigned char *FrameGenerator_Get(void);

#define BITMAP_ARRAY_SIZE   (4)
#define FRAMERATE           (60)

static HBITMAP hBitmapMask;
static HBITMAP hBitmapArray[BITMAP_ARRAY_SIZE];

struct dispinfo {
    int width;
    int height;
};

struct dispinfo g_dispinfo;

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    InvalidateRect(hwnd, NULL, FALSE); // false = not use brush for background
}

// custom event supported
// set dispinfo
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    HDC hdcMem;
    BITMAP bitmap;
    unsigned char* bgra = NULL;
    int i;
    BLENDFUNCTION ftn = { 0 };

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        
        g_dispinfo.width = 500;
        g_dispinfo.height = 500;
    
        hdc = GetDC(hWnd);

        hBitmapMask = CreateCompatibleBitmap(hdc, g_dispinfo.width, g_dispinfo.height);

        for (i = 0; i < BITMAP_ARRAY_SIZE; i++)
            hBitmapArray[i] = CreateCompatibleBitmap(hdc, g_dispinfo.width, g_dispinfo.height);

        ReleaseDC(hWnd, hdc);

        SetTimer(hWnd, 0, (1000/FRAMERATE), TimerProc);

        break;
    case WM_SIZE:
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        hdcMem = CreateCompatibleDC(hdc);

        bgra = FrameGenerator_Get();
        SetBitmapBits(hBitmapArray[0], 500*500*4, bgra);

        // select correct image
        SelectObject(hdcMem, hBitmapArray[0]);
        
        BitBlt(hdc, 0, 0, 500, 500, hdcMem, 0, 0, SRCCOPY);

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
        wprintf(L"filename = %s\n", filename);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int TransferWindowRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("Transfer");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

