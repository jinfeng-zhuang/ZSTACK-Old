#include <Windows.h>
#include <zstack/zstack.h>

static void Draw(HDC hdc, int width, int height, HBRUSH brush, struct ringbuffer *desc)
{
    HBRUSH oldBrush;
    //int width;
    //int height;
    struct ringbuffer normalize;
    float scale;
    
    //GetClientRect(hdc, &rect);
    
    //width = rect.right - rect.left;
    //height = rect.bottom - rect.top;
    
    normalize.start = 0;
    normalize.end = desc->end - desc->start;
    normalize.wp = desc->wp - desc->start;
    normalize.rp = desc->rp - desc->start;    
    
    scale = (float)normalize.end / width;

    normalize.wp = (unsigned int)(normalize.wp / scale);
    normalize.rp = (unsigned int)(normalize.rp / scale);
    normalize.end = (unsigned int)(normalize.end / scale);
    
    Rectangle(hdc, 0, 0, normalize.end, height);
    
    oldBrush = (HBRUSH)SelectObject(hdc, brush);
    
    if (normalize.wp >= normalize.rp) {
        Rectangle(hdc, normalize.rp, 0, normalize.wp, height);
    }
    else {
        Rectangle(hdc, normalize.rp, 0, normalize.end, height);
        Rectangle(hdc, normalize.start, 0, normalize.wp, height);
    }
    
    SelectObject(hdc, oldBrush);
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hPaper;
    static HDC hDeck;
    static HBRUSH hBrush;
    static struct ringbuffer ringbuffer = {0, 0, 0, 0};
    
    HDC hdcWindow;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    HBITMAP hPaperOld;
    RECT rect;
    struct ringbuffer ringbuf = {0, 100, 30, 50};

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

        SendMessage(hWnd, WM_USER, (WPARAM)&ringbuf, NULL);

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

        Draw(hDeck, rect.right - rect.left, rect.bottom - rect.top, hBrush, &ringbuffer);
        
        BitBlt(hdcWindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDeck, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
        break;
    case WM_USER:
        memcpy((unsigned char *)&ringbuffer, (unsigned char *)wParam, sizeof(struct ringbuffer));
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

int RingBufferWindow_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("RingBufferWindow");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

