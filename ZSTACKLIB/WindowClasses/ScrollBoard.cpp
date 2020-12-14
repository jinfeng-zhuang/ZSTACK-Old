#include <Windows.h>
#include <zstack.h>

HBITMAP hBitmap;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HDC hdcMem;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int pos;
    int cx, cy;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        hBitmap = (HBITMAP)LoadImage(NULL, L"demo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
        break;
    case WM_SIZE:
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        SetScrollRange(hWnd, SB_HORZ, 0, cx, FALSE);
        SetScrollRange(hWnd, SB_VERT, 0, cy, FALSE);
        SetScrollPos(hWnd, SB_HORZ, 0, TRUE);
        SetScrollPos(hWnd, SB_VERT, 0, TRUE);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_VSCROLL:
        pos = GetScrollPos(hWnd, SB_VERT);
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            pos -= 10;
            break;
        case SB_LINEDOWN:
            pos += 10;
            break;
        case SB_PAGEUP:
            break;
        case SB_PAGEDOWN:
            break;
        case SB_THUMBTRACK:
            pos = HIWORD(wParam);
            break;
        }
        SetScrollPos(hWnd, SB_VERT, pos, TRUE);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_HSCROLL:
        pos = GetScrollPos(hWnd, SB_HORZ);
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            pos -= 10;
            break;
        case SB_LINEDOWN:
            pos += 10;
            break;
        case SB_PAGEUP:
            break;
        case SB_PAGEDOWN:
            break;
        case SB_THUMBTRACK:
            pos = HIWORD(wParam);
            break;
        }
        SetScrollPos(hWnd, SB_HORZ, pos, TRUE);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT:
        cy = GetScrollPos(hWnd, SB_VERT);
        cx = GetScrollPos(hWnd, SB_HORZ);
        hdc = BeginPaint(hWnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);
        BitBlt(hdc, -cx, -cy, 400, 400, hdcMem, 0, 0, SRCCOPY);
        DeleteDC(hdcMem);
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

int ScrollBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("ScrollBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}
