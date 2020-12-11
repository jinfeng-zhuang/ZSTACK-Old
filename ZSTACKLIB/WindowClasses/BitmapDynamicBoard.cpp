#include <Windows.h>
#include "common.h"
#include <math.h>
#include <gdiplus.h>

using namespace Gdiplus;

#define CHAR_NUM    11
#define POINT_NUM   20
#define PI  (3.1415926)

static HBITMAP hBitmap;

static POINT circle[POINT_NUM];
static WCHAR text[CHAR_NUM + 1] = L"为什么忧愁它总围绕着我";

static int offset = 0;

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    offset++;

    offset = offset % POINT_NUM;

    InvalidateRect(hwnd, NULL, TRUE);
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int i;
    int radius;
    int angle;
    POINT center = { 100, 100 };
    LOGFONT logicfont;
    HFONT hfont;
    int index;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        hBitmap = (HBITMAP)LoadImage(NULL, L"demo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

        angle = 360 / (POINT_NUM);
        radius = 100;
        for (i = 0; i < POINT_NUM; i++) {
            circle[i].x = radius * cos(angle * i * PI / 180);
            circle[i].y = radius * sin(angle * i * PI / 180);
        }
        SetTimer(hWnd, 0, 100, TimerProc);
        break;
    case WM_SIZE:
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        break;
    case WM_PAINT:
        printf("paint\n");
        hdc = BeginPaint(hWnd, &ps);

        memset(&logicfont, 0, sizeof(logicfont));
        wsprintf(logicfont.lfFaceName, TEXT("黑体"));
        logicfont.lfHeight = 20;
        hfont = CreateFontIndirect(&logicfont);
        SelectObject(hdc, hfont);

        if (hBitmap) {
            HDC hdcMem;
            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hBitmap);
            BitBlt(hdc, 0, 0, 400, 400, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);

            SetBkMode(hdc, TRANSPARENT);
            for (i = 0; i < CHAR_NUM; i++) {
                index = (i + offset) % POINT_NUM;
                TextOut(hdc, circle[index].x + center.x, circle[index].y + center.y, &text[i], 1);
            }
        }
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

int BitmapDynamicBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("BitmapDynamicBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}
