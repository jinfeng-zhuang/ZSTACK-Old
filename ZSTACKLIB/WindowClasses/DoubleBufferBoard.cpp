#include <Windows.h>
#include "common.h"

static HDC hdc_a, hdc_b, hdc_c;
static HBITMAP bitmap_a, bitmap_b;
static int flag = 0;

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    flag = ~flag;

    InvalidateRect(hwnd, NULL, FALSE); // 擦除背景会导致闪烁，不擦除就好了。
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int i;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        hdc = GetDC(hWnd);
        bitmap_a = CreateBitmap(400, 400, 32, 1, NULL);
        bitmap_b = CreateBitmap(400, 400, 32, 1, NULL);

        hdc_a = CreateCompatibleDC(hdc);
        hdc_b = CreateCompatibleDC(hdc);
        hdc_c = CreateCompatibleDC(hdc);

        SelectObject(hdc_a, bitmap_a);
        SelectObject(hdc_b, bitmap_b);
        SelectObject(hdc_c, bitmap_b);

        TextOut(hdc_a, 0, 0, TEXT("传说"), wcslen(TEXT("传说")));
        TextOut(hdc_b, 0, 0, TEXT("史诗"), wcslen(TEXT("史诗")));

        ReleaseDC(hWnd, hdc);

        SetTimer(hWnd, 0, 33, TimerProc);
        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        if (flag) {
            BitBlt(hdc, 0, 0, 400, 400, hdc_a, 0, 0, SRCCOPY);
        }
        else {
            BitBlt(hdc, 0, 0, 400, 400, hdc_c, 0, 0, SRCCOPY);
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_KEYDOWN:
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

int DoubleBufferBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("DoubleBufferBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}
