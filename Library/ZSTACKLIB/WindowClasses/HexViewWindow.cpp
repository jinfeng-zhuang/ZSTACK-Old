#include <Windows.h>

#include <zstack.h>

static char content[512] = "Hello World";

static void Draw(HDC hdc, int width, int height, HBRUSH brush, struct YUVWindowData *data)
{
    int i;

    log_info("Draw\n");

    Rectangle(hdc, 0, 0, width, height);

    for (i = 0; i < 16; i++) {
        TextOut(hdc, 0, i * 20, TEXT("FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF"), 16*3 - 1);
    }
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hPaper;
    static HDC hDeck;
    static HBRUSH hBrush;
    
    HDC hdcWindow;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    HBITMAP hPaperOld;
    RECT rect;

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rect);

        hdcWindow = GetDC(hWnd);
        
        hDeck = CreateCompatibleDC(hdcWindow);
        hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);
        hBrush = CreateSolidBrush(RGB(0, 0, 0xFF));

        // select a paper to paint
        SelectObject(hDeck, hPaper);

        ReleaseDC(hWnd, hdcWindow);
        break;
    case WM_SIZE:
        GetClientRect(hWnd, &rect);

        hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);

        // select a new 'size' paper to paint
        hPaperOld = (HBITMAP)SelectObject(hDeck, hPaper);
        
        DeleteObject(hPaperOld);
        break;

    case WM_PAINT:
        hdcWindow = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rect);
        //Draw(hDeck, rect.right - rect.left, rect.bottom - rect.top, hBrush, &data);
        BitBlt(hdcWindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDeck, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        break;
    case WM_USER:
        //memcpy((unsigned char *)&data, (unsigned char *)wParam, sizeof(struct YUVWindowData));
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

int HexView_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("HexView");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}
