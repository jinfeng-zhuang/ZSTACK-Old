#include <Windows.h>
#include "common.h"

static int offset;

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    int w, h;
    RECT rect;

    offset++;

    if (offset >= 180)
        offset = 90;

    GetWindowRect(hwnd, &rect);

    w = rect.right - rect.left;
    h = rect.bottom - rect.top;

    //SetWindowPos(hwnd, NULL, sin(offset * PI / 180) * 300, 0, w, h, 0); // offset ÊÇ»¡¶È
}

static POINT origin;
static POINT cur;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int i;
    static POINT apt[4] = { {0, 0}, {0,100}, {100,0}, {300, 300} };
    POINT arrow[3];

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        origin.x = LOWORD(lParam);
        origin.y = HIWORD(lParam);

        printf("%d %d\n", origin.x, origin.y);

        break;
    case WM_LBUTTONUP:
        apt[3] = apt[2] = apt[1] = apt[0] = { 0, 0 };
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_MOUSEMOVE:
        if (!GetAsyncKeyState(VK_LBUTTON))
            break;

        cur.x = LOWORD(lParam);
        cur.y = HIWORD(lParam);

        apt[0].x = origin.x;
        apt[0].y = origin.y;

        apt[3].x = cur.x;
        apt[3].y = cur.y;

        if (abs(cur.x - origin.x) > abs(cur.y - origin.y)) {
            apt[1].x = (apt[0].x + apt[3].x) / 2;
            apt[1].y = (apt[0].y + apt[3].y) / 2 + 50;
            apt[2] = apt[1];
        }
        else {
            apt[1].x = (apt[0].x + apt[3].x) / 2 + 50;
            apt[1].y = (apt[0].y + apt[3].y) / 2;
            apt[2] = apt[1];
        }

        arrow[0] = apt[0];

        InvalidateRect(hWnd, NULL, TRUE);

        break;
    case WM_CREATE:

        SetTimer(hWnd, 0, 10, TimerProc);
        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        PolyBezier(hdc, apt, 4);

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

int CurveBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("CurveBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}
